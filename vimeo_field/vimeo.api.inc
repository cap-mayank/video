<?php

/**
 * @file
 * This vimeo api file to communicate with vimoe library.
 */

/**
 * Connect to the Vimeo API.
 *
 * @param object $account
 *   An authenticated vimeo_field_connect object to be used to authenticate
 *   against Vimeo.
 *
 * @return Object
 *   a Vimeo object ready to be used to query the Vimeo API or FALSE.
 */
function vimeo_field_connect($account = NULL) {
  global $user;
  $vimeo_consumer_key = variable_get('vimeo_field_consumer_key', NULL);
  $vimeo_consumer_secret = variable_get('vimeo_field_consumer_secret', NULL);

  $oauth_request_token = variable_get('vimeo_field_oauth_access_token', NULL);
  $oauth_request_token_secret = variable_get('vimeo_field_oauth_access_token_secret', NULL);

  if ($vimeo_consumer_key && $vimeo_consumer_secret && $oauth_request_token && $oauth_request_token_secret) {
    require_once drupal_get_path('module', 'vimeo_field') . '/vimeo.lib.php';
    return new phpVimeo($vimeo_consumer_key, $vimeo_consumer_secret, $oauth_request_token, $oauth_request_token_secret);
  }
  elseif ($vimeo_consumer_key && $vimeo_consumer_secret) {
    drupal_set_message(t('There is an error in vimeo API please try again latter.'));
    if ($user->id == 1) {
      redirect('admin/config/services/vimeo/settings');
    }
    else {
      redirect('/');
    }
  }
  return FALSE;
}

/**
 * Authounticate user form Vimeo API.
 */
function vimeo_field_auth() {
  $oauth_request_token = variable_get('oauth_field_request_token', NULL);
  $oauth_request_token_secret = variable_get('oauth_field_request_token_secret', NULL);
  $vimeo_consumer_key = variable_get('vimeo_field_consumer_key', NULL);
  $vimeo_consumer_secret = variable_get('vimeo_field_consumer_secret', NULL);

  if ($oauth_request_token && $oauth_request_token_secret) {
    require_once drupal_get_path('module', 'vimeo_field') . '/vimeo.lib.php';
    $vimeo = new phpVimeo($vimeo_consumer_key, $vimeo_consumer_secret);
    $vimeo->setToken($_SESSION['oauth_request_token'], $_SESSION['oauth_request_token_secret']);
  }
  else {
    drupal_set_message(t('There is an error in vimeo API please try again latter.'));
  }
}
