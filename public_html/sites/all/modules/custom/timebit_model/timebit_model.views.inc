<?php

/**
 * Implements hook_views_data()
 */
function timebit_model_views_data() {
  $data = array();

  $data['timebit_logs']['table']['group']  = t('timebit_logs');

  $data['timebit_logs']['table']['base'] = array(
    'field' => 'pid',
    'title' => t('timebit_logs'),
    'help' => t('timebit_logs'),
  );

  $data['timebit_logs']['tlid'] = array(
    'title' => t('tlid'),
    'help' => t('Time Log ID'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['timebit_logs']['uid'] = array(
    'title' => t('UID'),
    'help' => t('UID'),
    'relationship' => array(
      'title' => t('User ID'),
      'help' => t('Relate time log to the user who created it.'),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('User ID'),
    ),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['timebit_logs']['timestamp_start'] = array(
    'title' => t('start'),
    'help' => t('start'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['timebit_logs']['timestamp_stop'] = array(
    'title' => t('stop'),
    'help' => t('stop'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  return $data;
}
