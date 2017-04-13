<?php

/**
 * @file
 * Views integration for the vimeo_field module.
 */

/**
 * Implements hook_views_plugin().
 */
function vimeo_field_views_plugins() {
  return array(
    'module' => 'vimeo_field',
    'style' => array(
      'vimeo_field_sharedplayer' => array(
        'title' => t('Vimeo shared player'),
        'theme' => 'vimeo_field_sharedplayer_view',
        'theme path' => drupal_get_path('module', 'vimeo_field') . '/includes',
        'path' => drupal_get_path('module', 'vimeo_field') . '/includes',
        'help' => t('Show a video player followed by the resulting nodes of this view. The nodes will play on the same player when clicked.'),
        'handler' => 'vimeo_field_style_plugin',
        'uses row plugin' => FALSE,
        'uses row class' => TRUE,
        'uses fields' => TRUE,
        'uses grouping' => FALSE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}
