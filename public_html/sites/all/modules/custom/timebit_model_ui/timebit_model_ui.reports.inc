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
    $uid = -1;
  }

  if (isset($_GET['date_from'])) {
    $timestamp_start = $_GET['date_from'];
  }
  else {
    $timestamp_start = 0;
  }

  if (isset($_GET['date_to'])) {
    $timestamp_stop = $_GET['date_to'];
  }
  else {
    $timestamp_stop = 0;
  }

  $data = timebit_model_log_get_by_date_and_uid($timestamp_start, $timestamp_stop, $uid);

  // Display results as table.
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

  $form['uid'] = array(
    '#type' => 'select',
    '#title' => t('User'),
    '#options' => _timebit_model_ui_reports_page_get_users(),
    '#default_value' => isset($_GET['uid']) ? $_GET['uid'] : -1,
  );

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

  drupal_goto('timebit/reports', array(
    'query' => array(
      'date_from' => strtotime($values['date_from']), // TODO - check timezone.
      'date_to' => strtotime($values['date_to']),
      'uid' => $values['uid'],
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
    t('UID'),
    t('Start'),
    t('Stop'),
    t('Work time'),
    t('Name'),

  );
  return $header;
}

/**
 * @param $data
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
    $time = gmdate("H:i:s", $seconds);
    $output .= '<div class="summary-item"><span class="user-name">' . $name . '</span>: <span class="user-time">' . $time . '</span></div>';
  }

  $output .= '</div>';

  return $output;
}