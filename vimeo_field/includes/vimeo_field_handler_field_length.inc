<?php

/**
 * @file
 * File vimeo_handler_field_length.inc..
 *
 * Provides a handler for displaying video length column.
 */

/**
 * ???
 */
class vimeo_field_handler_field_length extends views_handler_field_node {

  /**
   * Render Vimeo API outoput. video length.
   */
  public function render($values) {

    $values = clone($values);
    $length = $values->{$this->field_alias};
    return theme('vimeo_video_length', array('length' => $length));
  }

}
