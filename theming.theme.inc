<?php

/**
 * @file
 * Theme function overrides.
 */

/**
 * Preprocess variables for form_element.
 *
 * Add needed default values and the following classes for the DIV container:
 * - form-item: Generic for all form elements.
 * - form-type-#type: The internal element #type.
 * - form-item-#name: The internal form element #name (usually derived from the
 *   $form structure and set via form_builder()).
 * - form-disabled: Only set if the form element is #disabled.
 */
function theming_preprocess_form_element(&$variables) {
  $element = &$variables['element'];

  // This function is invoked as theme wrapper, but the rendered form element
  // may not necessarily have been processed by form_builder().
  $element += array(
    '#title_display' => 'before',
    '#wrapper_attributes' => [],
    '#description_attributes' => [],
  );
  $element['#description_attributes'] += ['class' => ['description']];
  // Add element #id for #type 'item'.
  if (isset($element['#markup']) && !empty($element['#id'])) {
    $element['#wrapper_attributes']['id'] = $element['#id'];
  }
  // Add element's #type and #name as class to aid with JS/CSS selectors.
  $element['#wrapper_attributes']['class'][] = 'form-item';
  if (!empty($element['#type'])) {
    $element['#wrapper_attributes']['class'][] = 'form-type-' . strtr($element['#type'], '_', '-');
  }
  if (!empty($element['#name'])) {
    $element['#wrapper_attributes']['class'][] = 'form-item-' . strtr($element['#name'], array(' ' => '-', '_' => '-', '[' => '-', ']' => ''));
  }
  // Add a class for disabled elements to facilitate cross-browser styling.
  if (!empty($element['#attributes']['disabled'])) {
    $element['#wrapper_attributes']['class'][] = 'form-disabled';
  }

  if ($classes = $element['#attributes']['class'] ?? NULL) {
    $element['#attributes']['class'] = array_combine($classes, $classes);
  }
  if ($classes = $element['#wrapper_attributes']['class'] ?? NULL) {
    $element['#wrapper_attributes']['class'] = array_combine($classes, $classes);
  }
  if ($classes = $element['#label_attributes']['class'] ?? NULL) {
    $element['#label_attributes']['class'] = array_combine($classes, $classes);
  }
  if ($classes = $element['#description_attributes']['class'] ?? NULL) {
    $element["#description_attributes"]['class'] = array_combine($classes, $classes);
  }
}


/**
 * Returns HTML for a form element.
 *
 * Each form element is wrapped in a DIV container having with attributes from
 * #wrapper_attributes.
 *
 * In addition to the element itself, the DIV contains a label for the element
 * based on the optional #title_display property, and an optional #description.
 *
 * The optional #title_display property can have these values:
 * - before: The label is output before the element. This is the default.
 *   The label includes the #title and the required marker, if #required.
 * - after: The label is output after the element. For example, this is used
 *   for radio and checkbox #type elements as set in system_element_info().
 *   If the #title is empty but the field is #required, the label will
 *   contain only the required marker.
 * - invisible: Labels are critical for screen readers to enable them to
 *   properly navigate through forms but can be visually distracting. This
 *   property hides the label for everyone except screen readers.
 * - attribute: Set the title attribute on the element to create a tooltip
 *   but output no label element. This is supported only for checkboxes
 *   and radios in form_pre_render_conditional_form_element(). It is used
 *   where a visual label is not needed, such as a table of checkboxes where
 *   the row and column provide the context. The tooltip will include the
 *   title and required marker.
 *
 * If the #title property is not set, then the label and any required marker
 * will not be output, regardless of the #title_display or #required values.
 * This can be useful in cases such as the password_confirm element, which
 * creates children elements that have their own labels and required markers,
 * but the parent element should have neither. Use this carefully because a
 * field without an associated label can cause accessibility challenges.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #title_display, #description, #id, #required,
 *     #children, #type, #name.
 *
 * @ingroup themeable
 */
function theming_form_element($variables) {
  $element = &$variables['element'];

  $output = '<div' . drupal_attributes($element['#wrapper_attributes']) . '>' . "\n";

  // If #title is not set, we don't display any label or required marker.
  if (!isset($element['#title'])) {
    $element['#title_display'] = 'none';
  }
  $prefix = isset($element['#field_prefix']) ? '<span class="field-prefix">' . $element['#field_prefix'] . '</span> ' : '';
  $suffix = isset($element['#field_suffix']) ? ' <span class="field-suffix">' . $element['#field_suffix'] . '</span>' : '';

  switch ($element['#title_display']) {
    case 'before':
    case 'invisible':
      $output .= ' ' . theme('form_element_label', $variables);
      $output .= ' ' . $prefix . $element['#children'] . $suffix . "\n";
      break;

    case 'after':
      $output .= ' ' . $prefix . $element['#children'] . $suffix;
      $output .= ' ' . theme('form_element_label', $variables) . "\n";
      break;

    case 'none':
    case 'attribute':
      // Output no label and no required marker, only the children.
      $output .= ' ' . $prefix . $element['#children'] . $suffix . "\n";
      break;
  }

  if (!empty($element['#description'])) {
    $output .= '<div' . drupal_attributes($element['#description_attributes']) . '>' . $element['#description'] . "</div>\n";
  }

  $output .= "</div>\n";

  return $output;
}

/**
 * Preprocess variables for the form_element_label.
 */
function theming_preprocess_form_element_label(&$variables) {
  $element = &$variables['element'];
  $element += [
    '#label_attributes' => [],
  ];
  // Style the label as class option to display inline with the element.
  if ($element['#title_display'] == 'after') {
    $element['#label_attributes']['class'][] = 'option';
  }
  // Show label only to screen readers to avoid disruption in visual flows.
  elseif ($element['#title_display'] == 'invisible') {
    $element['#label_attributes']['class'][] = 'element-invisible';
  }
  if (!empty($element['#id'])) {
    $element['#label_attributes']['for'] = $element['#id'];
  }
}

/**
 * Returns HTML for a form element label and required marker.
 *
 * Form element labels include the #title and a #required marker. The label is
 * associated with the element itself by the element #id. Labels may appear
 * before or after elements, depending on theme_form_element() and
 * #title_display.
 *
 * This function will not be called for elements with no labels, depending on
 * #title_display. For elements that have an empty #title and are not required,
 * this function will output no label (''). For required elements that have an
 * empty #title, this will output the required marker alone within the label.
 * The label will use the #id to associate the marker with the field that is
 * required. That is especially important for screenreader users to know
 * which field is required.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #required, #title, #id, #value, #description.
 *
 * @ingroup themeable
 */
function theming_form_element_label($variables) {
  $element = $variables['element'];
  // This is also used in the installer, pre-database setup.
  $t = get_t();

  // If title and required marker are both empty, output no label.
  if ((!isset($element['#title']) || $element['#title'] === '') && empty($element['#required'])) {
    return '';
  }

  // If the element is required, a required marker is appended to the label.
  $required = !empty($element['#required']) ? theme('form_required_marker', array('element' => $element)) : '';

  $title = filter_xss_admin($element['#title']);


  // The leading whitespace helps visually separate fields from inline labels.
  return ' <label' . drupal_attributes($element['#label_attributes']) . '>' . $t('!title !required', array('!title' => $title, '!required' => $required)) . "</label>\n";
}
