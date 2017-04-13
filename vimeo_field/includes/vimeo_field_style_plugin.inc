<?php

/**
 * @file
 * Provide the views Vimeo shared player object with default options and form.
 */

/**
 * Style plugin to render each embed code in a single Vimeo player.
 *
 * @ingroup views_style_plugins
 */
class vimeo_field_style_plugin extends views_plugin_style {

  /**
   * Vimeo video option list.
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['video_id_field'] = array('default' => NULL);
    $options['title_field'] = array('default' => NULL);
    $options['width'] = array('default' => 400);
    $options['height'] = array('default' => 300);

    return $options;
  }

  /**
   * Return the form for configuring the style plugin options.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $handlers = $this->display->handler->get_handlers('field');
    if (empty($handlers)) {
      $form['error_markup'] = array(
        '#markup' => '<div class="error messages">' . t('You need at least one field before you can configure the shared player settings.') . '</div>',
      );
      return;
    }

    $form['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Player width'),
      '#default_value' => $this->options['width'],
    );

    $form['height'] = array(
      '#type' => 'textfield',
      '#title' => t('Player height'),
      '#default_value' => $this->options['height'],
    );

    // We need to determine what fields are possible vimeo fields for the
    // style to use for embed codes. Unfortunately, the field info in the view
    // object doesn't contain field types. Instead, generate a list of all
    // vimeo fields, and compare that with the fields that are actually in the
    // view.
    $possible_vimeo_fields = array();
    $fields_info = field_info_instances();
    foreach ($fields_info as $bundles) {
      foreach ($bundles as $bundle) {
        foreach ($bundle as $field_name => $field) {
          $field_info = field_info_field($field_name);
          if ($field_info['type'] == 'vimeo') {
            $possible_vimeo_fields[$field['field_name']] = $field['field_name'];
          }
        }
      }
    }

    // Create an array of allowed columns from the data we know:
    $field_names = array_intersect_key($this->display->handler->get_field_labels(), $possible_vimeo_fields);
    $form['video_id_field'] = array(
      '#type' => 'select',
      '#title' => t('video Id field'),
      '#description' => t('Select the field that contains the embed code value for the video'),
      '#options' => $field_names,
      '#default_value' => isset($this->options['video_id_field']) ? $this->options['video_id_field'] : array_shift(array_keys($field_names)),
      '#required' => FALSE,
    );

    // Also let the user select what field to use as the title of each video.
    $form['title_field'] = array(
      '#type' => 'select',
      '#title' => t('Title field'),
      '#description' => t('Select the field that contains the title for each video'),
      '#options' => $this->display->handler->get_field_labels(),
      '#default_value' => isset($this->options['title_field']) ? $this->options['title_field'] : NULL,
      '#requred' => TRUE,
    );
  }

  /**
   * Ensure that the pager option is disabled.
   *
   * As this makes no sense with the shared player style.
   */
  public function validate() {
    $errors = parent::validate();
    if ($this->display->handler->use_pager()) {
      $errors[] = t('The Vimeo shared player style cannot be used with a pager. Disable the "Use pager" option for this display.');
    }
    return $errors;
  }

}
