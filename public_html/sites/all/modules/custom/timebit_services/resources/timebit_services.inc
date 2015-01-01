<?php

/**
 * @file
 * Display event statistics.
 */

/**
 *
 * @return boolean
 */
function timebit_services_services_access() {
  return  TRUE;
}

function timebit_services_card_createlog($card_code, $hash) {
  // Check hash.
  if (!timebit_services_hash_is_correct($hash)) {
    return FALSE;
  }

  // Check card number.
  $card_code_object = timebit_services_users_cards_load_by_card_code($card_code);
  if (!$card_code_object) {
    return services_error(t('Card not exists in system'), 1);
  }

  // Create log.
  $account = user_load($card_code_object->uid);
  timebit_model_log_switch($account);

  $data = array(
    'user_work_status' => timebit_model_log_get_user_status($account)
  );

  return $data;
}
