<?php

namespace Drupal\theming;

use Upal\DrupalUnitTestCase;

/**
 * Test theming of form elements using the overridden theme functions.
 */
class FormTest extends DrupalUnitTestCase {

  /**
   * Include the theme file.
   */
   public function setUp() : void {
     parent::setUp();
     require_once drupal_get_path('module', 'theming') . '/theming.theme.inc';
   }

  /**
   * Test rendering a wrapper-class.
   */
  public function testWrapperAndLabelClass() {
    $element['#type'] = 'textfield';
    $element['#title'] = 'Label';
    $element['#name'] = 'test1';
    $element['#id'] = 'edit-test1';
    $element['#wrapper_attributes']['class'][] = 'wrapper-class';
    $element['#label_attributes']['class'][] = 'label-class';
    $element['#children'] = 'rendered element';
    $str = drupal_render($element);
    $expected = <<<HTML
<div class="wrapper-class form-item form-type-textfield form-item-test1">
  <label class="label-class" for="edit-test1">Label </label>
 <input type="text" id="edit-test1" name="test1" size="60" maxlength="128" class="form-text" />
</div>

HTML;
    $this->assertEqual($expected, $str);
  }

  /**
   * Test improvements to the form element processing.
   */
  public function testFormElementPreprocess() {
    $element['#attributes']['class'][] = 'element-class';
    $element['#wrapper_attributes']['class'][] = 'wrapper-class';
    $element['#label_attributes']['class'][] = 'label-class';
    $vars['element'] = &$element;
    theming_preprocess_form_element($vars);

    $this->assertEquals([
      'element-class' => 'element-class',
    ], $element['#attributes']['class']);
    $this->assertEquals([
      'wrapper-class' => 'wrapper-class',
      'form-item' => 'form-item',
    ], $element['#wrapper_attributes']['class']);
    $this->assertEquals([
      'label-class' => 'label-class',
    ], $element['#label_attributes']['class']);
  }

}
