<?php
/**
 * @file
 * Pages callbacks.
 */

/**
 * Module settings.
 * URL: /admin/application/mobileapi_hash
 */
function timebit_services_hash_settings_page() {
  $form = array();

  $form['mobileapi_hash'] = array(
    '#type' => 'textfield',
    '#title' => t('Hash'),
    '#default_value' => variable_get('mobileapi_hash', TIMEBIT_SERVICES_HASH_DEFAULT),
    '#description' => t('Default value') . ': ' . TIMEBIT_SERVICES_HASH_DEFAULT,
  );

  return system_settings_form($form);
}
