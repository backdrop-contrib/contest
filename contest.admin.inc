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
  $form['host'] = array(
    '#type'              => 'textfield',
    '#title'             => t('Host'),
    '#autocomplete_path' => 'contest/usr-autocomplete',
    '#default_value'     => db_query_range("SELECT name FROM {users} WHERE uid = :uid", 0, 1, array(':uid' => variable_get('contest_host_uid', 1)))->fetchField(),
    '#size'              => 30,
  );
  $form['contest_host_title'] = array(
    '#title'         =>  t('Host Title'),
    '#description'   => t("The host's display title."),
    '#type'          => 'textfield',
    '#default_value' => variable_get('contest_host_title', 'Contest Host'),
    '#size'          => 30,
    '#maxlength'     => 60,
    '#required'      => TRUE,
    '#weight'        => 40,
  );
  $form['contest_min_age'] = array(
    '#title'         =>  t('Minumum Age'),
    '#description'   => t("The minimum age required to enter a contest."),
    '#type'          => 'select',
    '#options'       => array_combine(range(1, 100), range(1, 100)),
    '#default_value' => variable_get('contest_min_age', 18),
    '#required'      => TRUE,
    '#weight'        => 50,
  );
  return system_settings_form($form);
}
/**
 * Custom validation function for the contest setting form.
 */
function contest_settings_form_validate($form, &$form_state) {

// Directory name doesn't match requirements.

  if (!preg_match('/^\w[\w\/]*\w$/', $form_state['values']['contest_export_dir'])) {
    form_set_error('contest_export_dir', t('The export directory can contain alpha-numeric characters, hyphens and slashes and must start and end with a alpha-numeric character.'));
  }
  if ($form_state['values']['contest_export_dir'] !== variable_get('contest_export_dir', '')) {
    $wrapper = file_stream_wrapper_get_instance_by_uri('private://');

// Private system path not configured.

    if (!$wrapper) {
      form_set_error('contest_export_dir', t("Drupal's file system path needs to be configured. Please go to !link to configure.", array('!link' => l('File System', 'admin/config/media/file-system', array('target' => '_blank')))));
    }
    $path = $wrapper->getDirectoryPath() . '/' . _contest_clean_path($form_state['values']['contest_export_dir']);

// Directory already in use.

    if (is_dir($path) || is_file($path)) {
      form_set_error('contest_export_dir', t('That export directory is already in use.'));
    }
  }
// Non-existent contest host.

  $host = user_load_by_name($form_state['values']['host']);

  if (empty($host->uid)) {
    form_set_error('host', t('No user with that name: @name.', array('@name' => $form_state['values']['host'])));
  }
// Non-existent contest host.

  if (!_contest_has_complete_profile($host, 'host')) {
    form_set_error('host', t('@name has an incomplete profile. If you have permission, you can click !link to edit.', array('@name' => $form_state['values']['host'], '!link' => l('here', "user/$host->uid/edit", array('attributes' => array('target' => '_blank'))))));
  }
}
/**
 * Custom submit function for the contest setting form.
 * Create the export directory.
 */
function contest_settings_form_submit($form, &$form_state) {
  $path = _contest_clean_path($form_state['values']['contest_export_dir']);
  $wrapper = file_stream_wrapper_get_instance_by_uri('private://');

// Convert the username to uid and save it and delete it's remnants from the form, (not sure if it matters but...).

  $uid = db_query_range("SELECT uid FROM {users} WHERE name = :name", 0, 1, array(':name' => $form_state['values']['host']))->fetchField();

  if ($uid) {
    variable_set('contest_host_uid', $uid);

    unset($form_state['input']['contest_export_dir']);
    unset($form_state['input']['host']);
    unset($form_state['values']['contest_export_dir']);
    unset($form_state['values']['host']);
  }
// Change the export directory if we need to.

  if ($path && $path === variable_get('contest_export_dir', '')) {
    return;
  }
  if (!$wrapper) {
    drupal_set_message("Drupal's file system private path needs to be configured. Please go to !link to configure.", array('!link' => l('File System', 'admin/config/media/file-system', array('target' => '_blank'))));
  }
  $old_dir = $wrapper->getDirectoryPath() . '/' . variable_get('contest_export_dir', '');
  _contest_purge_dir("$old_dir/*", 0);
  chmod($old_dir, 0777);
  rmdir($old_dir);

  _contest_create_export_directory($path);

  $form_state['values']['contest_export_dir'] = $path;
}
