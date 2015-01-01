<?php
/**
 * @file
 * timebit_views.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function timebit_views_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'timebit_logs_my_logs';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'timebit_logs';
  $view->human_name = 'timebit_logs_my_logs';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'My time logs';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'tlid' => 'tlid',
    'timestamp_start' => 'timestamp_start',
    'timestamp_stop' => 'timestamp_stop',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'tlid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'timestamp_start' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'timestamp_stop' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: timebit_logs: User ID */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'timebit_logs';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: timebit_logs: tlid */
  $handler->display->display_options['fields']['tlid']['id'] = 'tlid';
  $handler->display->display_options['fields']['tlid']['table'] = 'timebit_logs';
  $handler->display->display_options['fields']['tlid']['field'] = 'tlid';
  /* Field: timebit_logs: start */
  $handler->display->display_options['fields']['timestamp_start']['id'] = 'timestamp_start';
  $handler->display->display_options['fields']['timestamp_start']['table'] = 'timebit_logs';
  $handler->display->display_options['fields']['timestamp_start']['field'] = 'timestamp_start';
  $handler->display->display_options['fields']['timestamp_start']['label'] = 'Start';
  $handler->display->display_options['fields']['timestamp_start']['date_format'] = 'custom';
  $handler->display->display_options['fields']['timestamp_start']['custom_date_format'] = 'Y-m-d H:i:s';
  /* Field: timebit_logs: stop */
  $handler->display->display_options['fields']['timestamp_stop']['id'] = 'timestamp_stop';
  $handler->display->display_options['fields']['timestamp_stop']['table'] = 'timebit_logs';
  $handler->display->display_options['fields']['timestamp_stop']['field'] = 'timestamp_stop';
  $handler->display->display_options['fields']['timestamp_stop']['label'] = 'Stop';
  $handler->display->display_options['fields']['timestamp_stop']['date_format'] = 'custom';
  $handler->display->display_options['fields']['timestamp_stop']['custom_date_format'] = 'Y-m-d H:i:s';
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid_current']['value'] = '1';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'timebit/my-logs';
  $translatables['timebit_logs_my_logs'] = array(
    t('Master'),
    t('My time logs'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('User ID'),
    t('tlid'),
    t('.'),
    t(','),
    t('Start'),
    t('Stop'),
    t('Page'),
  );
  $export['timebit_logs_my_logs'] = $view;

  return $export;
}
