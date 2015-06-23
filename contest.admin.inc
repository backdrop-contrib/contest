<?php

/**
 * @file
 * Admin page callbacks for the contest module.
 */

/**
 * Menu callback to configure module settings.
 */
function contest_settings_form() {
  $periods = array(
    2147483646    => t('Can only win once.'),
    (7 * 86400)   => t('One week between wins.'),
    (30 * 86400)  => t('One month between wins.'),
    (90 * 86400)  => t('Three months between wins.'),
    (365 * 86400) => t('One year between wins.'),
  );
  $form['#submit'][] = 'contest_settings_form_submit';
  $form['#validate'][] = 'contest_settings_form_validate';
  $form['desc'] = array(
    '#type'   => 'item',
    '#value'  => t('Configuration options for the contest module.'),
    '#weight' => 0,
  );
  $form['contest_export_dir'] = array(
    '#title'         => t('Export Directory'),
    '#description'   => t('The export directory under files/, (no leading of trailing slash).'),
    '#type'          => 'textfield',
    '#default_value' => variable_get('contest_export_dir', 'contest_export'),
    '#size'          => 30,
    '#maxlength'     => 30,
    '#required'      => TRUE,
    '#weight'        => 10,
  );
  $form['contest_hog_days'] = array(
    '#title'         =>  t('Days Between Wins'),
    '#type'          => 'select',
    '#options'       => $periods,
    '#default_value' => variable_get('contest_hog_days', 90),
    '#required'      => TRUE,
    '#weight'        => 20,
  );
  $form['contest_host_uid'] = array(
    '#title'         =>  t('Host User ID'),
    '#type'          => 'textfield',
    '#default_value' => variable_get('contest_host_uid', 1),
    '#size'          => 30,
    '#maxlength'     => 6,
    '#required'      => TRUE,
    '#weight'        => 30,
  );
  $form['contest_host_title'] = array(
    '#title'         =>  t('Host Title'),
    '#type'          => 'textfield',
    '#default_value' => variable_get('contest_host_title', 'Contest Host'),
    '#size'          => 30,
    '#maxlength'     => 60,
    '#required'      => TRUE,
    '#weight'        => 40,
  );
  $form['contest_host_county'] = array(
    '#title'         =>  t('Host Country'),
    '#description'   => t("I'm not sure this will work in countries besides the US."),
    '#type'          => 'select',
    '#options'       => array(),//_contest_get_countries(),
    '#default_value' => variable_get('contest_host_county', 'US'),
    '#required'      => TRUE,
    '#weight'        => 50,
  );
  return system_settings_form($form);
}
/**
 * Custom validation function for the contest setting form.
 */
function contest_settings_form_validate($form, &$form_state) {
  if (!preg_match('/^\w[\w\/]*\w$/', $form_state['values']['contest_export_dir'])) {
    form_set_error('contest_export_dir', t('The export directory can contain alpha-numeric characters, hyphens and slashes and must start and end with a alpha-numeric character.'));
  }
  if ($form_state['values']['contest_export_dir'] !== variable_get('contest_export_dir', '')) {
    $path = file_directory_path() . '/' . _contest_clean_path($form_state['values']['contest_export_dir']);
    if (is_dir($path) || is_file($path)) {
      form_set_error('contest_export_dir', t('That export directory is already in use.'));
    }
  }
  if (!db_result(db_query('SELECT 1 FROM {users} WHERE uid = %d', $form_state['values']['contest_host_uid']))) {
    form_set_error('contest_host_uid', t('No user with ID: @uid.', array('@uid' => $form_state['values']['contest_host_uid'])));
  }
}
/**
 * Custom submit function for the contest setting form.
 * Create the export directory.
 */
function contest_settings_form_submit($form, &$form_state) {
  $now = time();
  $path = _contest_clean_path($form_state['values']['contest_export_dir']);

  if ($path && $path === variable_get('contest_export_dir', '')) {
    return;
  }
  $old_dir = file_directory_path() . '/' . variable_get('contest_export_dir', '');
  _contest_purge_dir("$old_dir/*", 0);
  chmod($old_dir, 0777);
  rmdir($old_dir);

  _contest_create_export_directory($path);

  $form_state['values']['contest_export_dir'] = $path;
}
