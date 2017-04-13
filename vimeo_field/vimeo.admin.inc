<?php

/**
 * @file
 * Vimeo settings form.
 */

/**
 * Menu callback function admin_form().
 */
function vimeo_field_admin_form($form, &$form_state) {
  global $base_url;
  $form['oauth'] = array(
    '#type' => 'fieldset',
    '#title' => t('OAuth Settings'),
    '#description' => t('To enable OAuth based access for vimeo, you must <a href="@url">register your application</a> with Vimeo and add the provided keys here.', array('@url' => 'https://developer.vimeo.com/apps')),
  );
  $vimeo_consumer_key = variable_get('vimeo_field_consumer_key', NULL);
  $vimeo_consumer_secret = variable_get('vimeo_field_consumer_secret', NULL);

  if ($vimeo_consumer_key && $vimeo_consumer_secret) {
    require_once drupal_get_path('module', 'vimeo_field') . '/vimeo.lib.php';
    $vimeo = new phpVimeo($vimeo_consumer_key, $vimeo_consumer_secret);

    $token = $vimeo->getRequestToken($base_url . '/vimeo/oauth');
    $_SESSION['vimeo_oauth']['token'] = $token;
    // Build authorize link.
    $authorize_link = $vimeo->getAuthorizeUrl($token['oauth_token'], 'write');
    $form['oauth']['oauth_token'] = array(
      '#type' => 'fieldset',
      '#title' => t('Get Oauth Token'),
      '#description' => t('To enable OAuth based access for vimeo, you must <a href="@url">verify app</a> with Vimeo.', array('@url' => $authorize_link)),
    );
    $form['vimeo_field_video_width'] = array(
      '#title' => t('Player width'),
      '#type' => 'textfield',
      '#description' => t('The width of the video player when viewing content.'),
      '#default_value' => variable_get('vimeo_field_video_width', 400),
      '#size' => 6,
      '#field_suffix' => ' ' . t('pixels'),
    );

    $form['vimeo_field_video_height'] = array(
      '#title' => t('Player height'),
      '#type' => 'textfield',
      '#description' => t('The height of the video player when viewing content.'),
      '#default_value' => variable_get('vimeo_field_video_height', 300),
      '#size' => 6,
      '#field_suffix' => ' ' . t('pixels'),
    );
  }
  $form['oauth']['callback_url'] = array(
    '#type' => 'item',
    '#title' => t('Callback URL'),
    '#markup' => url('vimeo/oauth', array('absolute' => TRUE)),
  );
  $form['oauth']['vimeo_field_consumer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('OAuth Consumer key'),
    '#default_value' => variable_get('vimeo_field_consumer_key', NULL),
    '#required' => TRUE,
  );
  $form['oauth']['vimeo_field_consumer_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('OAuth Consumer secret'),
    '#default_value' => variable_get('vimeo_field_consumer_secret', NULL),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Wrapper to call drupal_form_submit() which wasn't required in D6.
 */
function vimeo_field_oauth_callback() {
  if (isset($_GET['denied']) || empty($_GET['oauth_token'])) {
    drupal_set_message(t('The connection to Vimeo failed. Please try again.'), 'error');
    global $user;
    if ($user->uid) {
      // User is logged in, was attempting to OAuth a Vimeo account.
      drupal_goto('admin/config/services/vimeo/settings');
    }
    else {
      // Anonymous user, redirect to front page.
      drupal_goto('<front>');
    }
  }
  $form_state['values']['oauth_token'] = $_GET['oauth_token'];
  drupal_form_submit('vimeo_field_oauth_callback_form', $form_state);
}

/**
 * Form builder function.
 *
 * In D6 this form was built in response to the
 * oauth return request from Vimeo, and the setting of
 * $form['#post'] seems to have caused the form to be validated and
 * processed.
 */
function vimeo_field_oauth_callback_form($form, &$form_state) {
  $form['#post']['oauth_token'] = $_GET['oauth_token'];
  $form['oauth_token'] = array(
    '#type' => 'hidden',
    '#default_value' => $_GET['oauth_token'],
  );
  return $form;
}

/**
 * Validate results from Vimeo OAuth return request.
 */
function vimeo_field_oauth_callback_form_validate($form, &$form_state) {
  $key = variable_get('vimeo_field_consumer_key', NULL);
  $secret = variable_get('vimeo_field_consumer_secret', NULL);

  if (isset($_SESSION['vimeo_oauth'])) {
    $form_state['vimeo_oauth'] = $_SESSION['vimeo_oauth'];
    unset($_SESSION['vimeo_oauth']);
  }
  else {
    form_set_error('oauth_token', t('Invalid Vimeo OAuth request1.'));
  }
  if (isset($form_state['vimeo_oauth']['token'])) {
    $token = $form_state['vimeo_oauth']['token'];
    if (!is_array($token) || !$key || !$secret) {
      form_set_error('oauth_token', t('Invalid Vimeo OAuth request2.'));
    }
    if ($token['oauth_token'] != $form_state['values']['oauth_token']) {
      form_set_error('oauth_token', t('Invalid OAuth token.'));
    }
  }
  else {
    form_set_error('oauth_token', t('Invalid Vimeo OAuth request.'));
  }

  require_once drupal_get_path('module', 'vimeo_field') . '/vimeo.lib.php';

  if ($vimeo = new phpVimeo($key, $secret, $token['oauth_token'], $token['oauth_token_secret'])) {
    // Collect oauth_verifier from url.
    if ($response = $vimeo->getAccessToken($_GET['oauth_verifier'])) {
      $form_state['vimeo_oauth']['response'] = $response;
    }
    else {
      form_set_error('oauth_token', t('Invalid Vimeo OAuth request.'));
    }
  }
  else {
    form_set_error('oauth_token', t('Invalid Vimeo OAuth request.'));
  }
}

/**
 * Handle a Vimeo OAuth return request and store the account creds in the DB.
 */
function vimeo_field_oauth_callback_form_submit($form, &$form_state) {
  // $key = variable_get('vimeo_field_consumer_key', NULL);.
  // $secret = variable_get('vimeo_field_consumer_secret', NULL);.
  $response = $form_state['vimeo_oauth']['response'];
  variable_set('vimeo_field_oauth_access_token', $response['oauth_token']);
  variable_set('vimeo_field_oauth_access_token_secret', $response['oauth_token_secret']);
  drupal_set_message(t('Vimeo authentication is success..'));
  $form_state['programmed'] = FALSE;
  $form_state['redirect'] = '/admin/config/services/vimeo/settings';
}
