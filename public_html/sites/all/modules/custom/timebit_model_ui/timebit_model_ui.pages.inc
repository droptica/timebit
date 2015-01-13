<?php

/**
 * Reports page callback.
 *
 * @return string
 */
function timebit_model_ui_reports_page() {
  $output = '';

  // Filter form.
  $form = drupal_get_form('timebit_model_ui_reports_page_filter_form');
  $output .= render($form);

  // Get data.

  if (isset($_GET['uid'])) {
    $uid = $_GET['uid'];
  }
  else {
    if (user_access('administer time logs')) {
      $uid = -1;
    }
    else {
      global $user;
      $uid = $user->uid;
    }

  }


  $default_date_stop = REQUEST_TIME + 24 * 60 * 60;
  $default_date_start = REQUEST_TIME - 30 * 24 * 60 * 60;

  if (isset($_GET['date_from'])) {
    $timestamp_start = $_GET['date_from'];
  }
  else {
    $timestamp_start = $default_date_start;
  }

  if (isset($_GET['date_to'])) {
    $timestamp_stop = $_GET['date_to'];
  }
  else {
    $timestamp_stop = $default_date_stop;
  }

  $data = timebit_model_log_get_by_date_and_uid($timestamp_start, $timestamp_stop, $uid);

  // Display results as table.
  $output .= '<h3 class="add-link">' . l(t('Add new time log'), TIMEBIT_MODEL_MODULE_URI . '/add') . '</h3>';
  $output .= _timebit_model_ui_reports_page_table_results($data);

  // Summary.
  $output .= _timebit_model_ui_reports_page_summary($data);

  return $output;
}

/**
 * @param $form
 * @param $form_state
 */
function timebit_model_ui_reports_page_filter_form($form, $form_state) {

  $dateformat = 'Y-m-d H:i';

  $default_date_to = date($dateformat, REQUEST_TIME + 24 * 60 * 60);

  $default_date_from = date($dateformat, REQUEST_TIME - 30 * 24 * 60 * 60);

  //$form['#method'] = 'get';

  $form['date_from'] = array(
    '#type' => 'date_popup',
    '#title' => t('Date from'),
    '#default_value' => isset($_GET['date_from']) ? date($dateformat, $_GET['date_from']) : $default_date_from,
    '#required' => TRUE,
    '#date_timezone' => date_default_timezone(),
    '#date_format' => $dateformat,
    '#date_year_range' => '-3:+3',
  );

  $form['date_to'] = array(
    '#type' => 'date_popup',
    '#title' => t('Date to'),
    '#default_value' => isset($_GET['date_to']) ? date($dateformat, $_GET['date_to']) : $default_date_to,
    '#required' => TRUE,
    '#date_timezone' => date_default_timezone(),
    '#date_format' => $dateformat,
    '#date_year_range' => '-3:+3',
  );


  if (user_access('administer time logs')) {
    $form['uid'] = array(
      '#type' => 'select',
      '#title' => t('User'),
      '#options' => _timebit_model_ui_reports_page_get_users(),
      '#default_value' => isset($_GET['uid']) ? $_GET['uid'] : -1,
    );
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );

  return $form;
}

/**
 * @param $form
 * @param $form_state
 */
function timebit_model_ui_reports_page_filter_form_submit($form, $form_state) {
  $values = $form_state['values'];

  if (isset($values['uid'])) {
    $uid = $values['uid'];
  }
  else {
    if (user_access('administer time logs')) {
      $uid = -1;
    }
    else {
      global $user;
      $uid = $user->uid;
    }
  }


  drupal_goto(TIMEBIT_MODEL_MODULE_ALL_LOGS_URI, array(
    'query' => array(
      'date_from' => strtotime($values['date_from']), // TODO - check timezone.
      'date_to' => strtotime($values['date_to']),
      'uid' => $uid,
    )
  ));
}

/**
 * Get users for select form.
 *
 * @return array
 */
function _timebit_model_ui_reports_page_get_users() {
  $ret = array();

  $ret[-1] = t('All');

  $query = db_select('users', 'u');
  $query->fields('u');
  $query->condition('uid', 0, '<>');

  $results = $query->execute();

  foreach ($results as $row) {
    $ret[$row->uid] = $row->name;
  }

  return $ret;
}

/**
 * Data as table.
 *
 */
function _timebit_model_ui_reports_page_table_results($data) {
  $output = '';

  if ($data) {

    $data_table = array();
    // Prepare data for table.
    foreach ($data as $row) {
      $row = (array) $row;

      $row['work_time'] =  gmdate("H:i:s", $row['timestamp_stop'] - $row['timestamp_start']);
      $row['timestamp_start'] = format_date($row['timestamp_start'], 'short');
      $row['timestamp_stop'] = format_date($row['timestamp_stop'], 'short');
      $row['name'] = l(user_load($row['uid'])->name, 'user/' . $row['uid']);

      if (user_access('administer time logs')) {
        $row['edit'] = l(t('Edit'), TIMEBIT_MODEL_MODULE_URI . '/' . $row['tlid'] . '/edit');
        $row['delete'] = l(t('Delete'), TIMEBIT_MODEL_MODULE_URI . '/' . $row['tlid'] . '/delete');
      }

      unset($row['uid']);

      $data_table[] = $row;
    }
    $header = _timebit_model_ui_reports_page_table_results_header();

    $output .= theme('table', array(
      'header' => $header,
      'rows' => $data_table,
      'attributes' => array('width' => '100%')
    ));
  }

  return $output;
}

/**
 * Get header for table.
 */
function _timebit_model_ui_reports_page_table_results_header() {
  $header = array(
    t('Time Log ID'),
    //t('UID'),
    t('Start'),
    t('Stop'),
    t('Description'),
    t('Work time'),
    t('Name'),
  );

  if (user_access('administer time logs')) {
    $header[] = t('Edit');
    $header[] = t('Delete');
  }
  return $header;
}

/**
 * Return summary.
 *
 * @param $data
 *
 * @return HTML
 */
function _timebit_model_ui_reports_page_summary($data) {
  $output = '';

  $summary = array();
  if ($data) {
    foreach ($data as $row) {
      if (isset($summary[$row->uid])) {
        $current_sum = $summary[$row->uid];
      }
      else {
        $current_sum = 0;
      }
      $summary[$row->uid] = $current_sum + ($row->timestamp_stop - $row->timestamp_start);
    }
  }

  // Output. TODO - to separate function.

  $output .= '<div class="summary">';
  $output .= '<h2 class="summary-header">' . t('Summary') . '</h2>';
  foreach ($summary as $uid => $seconds) {
    $name = user_load($uid)->name;
    $time = timebit_model_seconds_to_hours($seconds);
    $output .= '<div class="summary-item"><span class="user-name">' . $name . '</span>: <span class="user-time">' . $time . ' ' . t('hours') . '</span></div>';
  }

  $output .= '</div>';

  return $output;
}


/**
 * Form - add/edit time log item.
 *
 * @param $form
 * @param $form_state
 * @return mixed
 */
function timebit_model_ui_item_form($form, &$form_state) {
  if (arg((TIMEBIT_MODEL_MODULE_URI_ARG_POSITION + 1)) == 'edit') {
    $entity = timebit_model_log_load(arg(TIMEBIT_MODEL_MODULE_URI_ARG_POSITION));
    $form['tlid'] = array('#type' => 'value', '#value' => $entity->tlid);
  }

  $users = _timebit_model_ui_reports_page_get_users();
  unset($users[-1]);

  $form['uid'] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t('User'),
    '#options' => $users,
    '#default_value' => isset($entity->uid) ? $entity->uid : '',
  );


  $dateformat = 'Y-m-d H:i';
  $default_date_start = date($dateformat);
  $default_date_stop = date($dateformat);

  $form['timestamp_start'] = array(
    '#type' => 'date_popup',
    '#title' => t('Date from'),
    '#default_value' => isset($entity->timestamp_start) ? date($dateformat, $entity->timestamp_start) : $default_date_start,
    '#required' => TRUE,
    '#date_timezone' => date_default_timezone(),
    '#date_format' => $dateformat,
    '#date_year_range' => '-3:+3',
  );

  $form['timestamp_stop'] = array(
    '#type' => 'date_popup',
    '#title' => t('Date to'),
    '#default_value' => isset($entity->timestamp_stop) ? date($dateformat, $entity->timestamp_stop) : $default_date_stop,
    '#required' => TRUE,
    '#date_timezone' => date_default_timezone(),
    '#date_format' => $dateformat,
    '#date_year_range' => '-3:+3',
  );

  $form['log_description'] = array(
    '#type' => 'textarea',
    '#required' => FALSE,
    '#title' => t('Log description'),
    '#default_value' => isset($entity->log_description) ? $entity->log_description : '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save time log'),
  );


  return $form;
}

/**
 * Form submit.
 */
function timebit_model_ui_item_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $values['timestamp_start'] = strtotime($values['timestamp_start']);
  $values['timestamp_stop'] = strtotime($values['timestamp_stop']);

  if (isset($values['tlid']) && is_numeric($values['tlid']) && timebit_model_log_load($values['tlid'])) {
    // Update existing time log.
    $row = new stdClass();
    $row->tlid = $values['tlid'];

    $row->uid = $values['uid'];
    $row->timestamp_start = $values['timestamp_start'];
    $row->timestamp_stop = $values['timestamp_stop'];
    $row->log_description = $values['log_description'];

    drupal_write_record('timebit_logs', $row, array('tlid'));
    drupal_set_message(t("Item has been updated"));
  }
  else {
    // Add new time log.
    $row = new stdClass();

    $row->uid = $values['uid'];
    $row->timestamp_start = $values['timestamp_start'];
    $row->timestamp_stop = $values['timestamp_stop'];
    $row->log_description = $values['log_description'];

    drupal_write_record('timebit_logs', $row);
    drupal_set_message(t("New item has been saved"));
  }

  $form_state['redirect'] = TIMEBIT_MODEL_MODULE_ALL_LOGS_URI;
}

/**
 * Delete time log form.
 *
 * @param $form
 * @param $form_state
 * @return mixed
 */
function timebit_model_ui_item_form_delete_form($form, &$form_state) {
  $form['tlid'] = array('#type' => 'value', '#value' => arg(TIMEBIT_MODEL_MODULE_URI_ARG_POSITION));
  $entity = timebit_model_log_load(arg(TIMEBIT_MODEL_MODULE_URI_ARG_POSITION));
  return confirm_form($form,
    t('Are you sure you want to delete log id: %logid?', array('%logid' => $entity->tlid)),
    TIMEBIT_MODEL_MODULE_ALL_LOGS_URI,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}


/**
 * Delete form submit handler.
 */
function timebit_model_ui_item_form_delete_form_submit($form, &$form_state) {
  $id = $form_state['values']['tlid'];
  timebit_model_log_delete($id);

  drupal_set_message(t('Item %id deleted.', array('%id' => $id )));

  $form_state['redirect'] = TIMEBIT_MODEL_MODULE_ALL_LOGS_URI;
}

/**
 * Page callback.
 */
function timebit_model_ui_live_status_page() {
  $output = '';

  $users = _timebit_model_ui_reports_page_get_users();
  if (isset($users[-1])) {
    unset($users[-1]);
  }

  $output .= '<div class="live-status-info">';
  foreach ($users as $uid => $name) {
    $account = user_load($uid);

    $status_code = timebit_model_log_get_user_status($account);

    if ($status_code == WORK_IN_PROGRESS) {
      $status_title = t('Working');
      $status_class = 'working';
      $log_in_progress = timebit_model_log_get_log_in_progress($account);
      $start_time = t('Start') . ': ' . format_date($log_in_progress->timestamp_start, 'short');

    }
    else {
      $status_title = t('Not working');
      $status_class = 'not-working';
      $start_time = '';
    }


    $output .= '<div class="live-status-item">';
    $output .= '<div class="live-status-item-user">' . l($name, 'user/' . $uid) . '</div>';
    $output .= '<div class="live-status-item-status ' . $status_class . ' ">' . $status_title . '</div>';
    $output .= '<div class="live-status-item-last-log">' . $start_time . '</div>';
    $output .= '</div>';
  }

  $output .= '</div>';

  return $output;
}