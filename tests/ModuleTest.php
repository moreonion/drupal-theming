<?php

namespace Drupal\theming;

use Upal\DrupalUnitTestCase;

/**
 * Test hook and callback implementations for the theming module.
 */
class ModuleTest extends DrupalUnitTestCase {

  /**
   * Test that it’s possible to override the type attribute of a <script> tag.
   */
  public function testPreRenderScriptsOverridesType() {
    $example_url = 'https://example.com/test.js';
    $elements['#items'] = [
      $example_url => [
        'type' => 'external',
        'defer' => FALSE,
        'attributes' => ['type' => 'module'],
        'data' => $example_url,
      ],
    ];
    $processed_elements = \theming_pre_render_scripts($elements);
    $this->assertEqual([
      'type' => 'module',
      'src' => $example_url,
    ], $processed_elements['scripts'][1]['#attributes']);
  }

}
