<?php

/**
 * @file
 * Admin page callbacks for the contest module.
 */

/**
 * Menu callback to configure module settings.
 */
function contest_settings_form() {
  $cfg = config('contest.settings');
  $periods = array(
    2147483646    => t('Can only win once.'),
    (7 * 86400)   => t('One week between wins.'),
    (30 * 86400)  => t('One month between wins.'),
    (90 * 86400)  => t('Three months between wins.'),
    (365 * 86400) => t('One year between wins.'),
  );
  $token_description = "
    <b>These are the tokens used by the Terms and Conditions:</b><br />
  	!host_link - A link to this site.<br />
  	!server_link - An alternat link to this site, (Apache configuration varies).<br />
  	@county - The host's country.<br />
  	@date_end - The date the contest ends.<br />
  	@date_notify - The date the winners will be notified by.<br />
  	@date_start - The date the contest starts.<br />
  	@host_address - The host's address.<br />
  	@host_business - The host's business.<br />
  	@host_city - The hosts's city.<br />
  	@host_name - The host's name.<br />
  	@host_state - The host's state<br />
  	@host_zip - The host's postal code, (zip).<br />
  	@host_title - The host's alternate name.<br />
  	@places - The number of places this contest is awarding.<br />
  	@timezone - The timezone that will be used for the contest.<br />
  ";
  $form['#submit'][] = 'contest_settings_form_submit';
  $form['#validate'][] = 'contest_settings_form_validate';
  $form['desc'] = array(
    '#type'   => 'item',
    '#value'  => t('Configuration options for the contest module.'),
    '#weight' => 0,
  );
  $form['export_dir'] = array(
    '#title'         => t('Export Directory'),
    '#description'   => t("The export directory under the site's private directory, (no leading of trailing slash)."),
    '#type'          => 'textfield',
    '#default_value' => $cfg->get('export_dir'),
    '#size'          => 30,
    '#maxlength'     => 30,
    '#required'      => TRUE,
    '#weight'        => 10,
  );
  $form['hog_days'] = array(
    '#title'         =>  t('Days Between Wins'),
    '#type'          => 'select',
    '#options'       => $periods,
    '#default_value' => $cfg->get('hog_days'),
    '#required'      => TRUE,
    '#weight'        => 20,
  );
  $form['host'] = array(
    '#type'              => 'textfield',
    '#title'             => t('Host'),
    '#autocomplete_path' => 'contest/usr-autocomplete',
    '#default_value'     => db_query_range("SELECT name FROM {users} WHERE uid = :uid", 0, 1, array(':uid' => $cfg->get('host_uid')))->fetchField(),
    '#size'              => 30,
  );
  $form['host_title'] = array(
    '#title'         =>  t('Host Title'),
    '#description'   => t("The host's display title."),
    '#type'          => 'textfield',
    '#default_value' => $cfg->get('host_title'),
    '#size'          => 30,
    '#maxlength'     => 60,
    '#required'      => TRUE,
    '#weight'        => 40,
  );
  $form['min_age'] = array(
    '#title'         =>  t('Minumum Age'),
    '#description'   => t("The minimum age required to enter a contest."),
    '#type'          => 'select',
    '#options'       => array_combine(range(1, 100), range(1, 100)),
    '#default_value' => $cfg->get('min_age'),
    '#required'      => TRUE,
    '#weight'        => 50,
  );
  $form['tnc'] = array(
    '#title'       => t('Terms & Conditions'),
    '#description' => t("The contest's terms and conditions."),
    '#type'        => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
    '#weight'      => 60,
  );
  $form['tnc']['contest_block_tnc_content'] = array(
    '#type'          => 'text_format',
    '#description'   => t($token_description),
    '#default_value' => _contest_get_block_body($cfg->get('block_tnc')),
    '#format'        => _contest_get_block_body_format($cfg->get('block_tnc')),
    '#required'      => FALSE,
  );
  $form['submit'] = array(
    '#type'   => 'submit',
    '#value'  => t('Save'),
    '#weight' => 100,
  );
  return $form;
}
/**
 * Custom submit function for the contest setting form.
 * Create the export directory.
 */
function contest_settings_form_submit($form, &$form_state) {
  $cfg = config('contest.settings');
  $path = _contest_clean_path($form_state['values']['export_dir']);
  $wrapper = file_stream_wrapper_get_instance_by_uri('private://');
  $fields = array(
    'hog_days',
    'host_title',
    'min_age',
  );
// Convert the username to uid and save it and delete it's remnants from the form, (not sure if it matters but...).

  $uid = db_query_range("SELECT uid FROM {users} WHERE name = :name", 0, 1, array(':name' => $form_state['values']['host']))->fetchField();

  if ($uid) {
     $cfg->set('host_uid', $uid);
  }
// Update the T&C content.

  $fields = array(
    'body'   => $form_state['values']['contest_block_tnc_content']['value'],
    'format' => $form_state['values']['contest_block_tnc_content']['format'],
  );
  db_update('block_custom')->fields($fields)->condition('bid', config_get('contest.settings', 'block_tnc'))->execute();

// .

  foreach ($fields as $field) {
    if (isset($form_state['values'][$field])) {
      $cfg->set($field, $form_state['values'][$field]);
    }
  }
// Change the export directory if we need to.

  if ($path && $path === config_get('contest.settings', 'export_dir')) {
    return $cfg->save();
  }
  if (!$wrapper) {
    backdrop_set_message("Backdrop's file system private path needs to be configured. Please go to !link to configure.", array('!link' => l('File System', 'admin/config/media/file-system', array('target' => '_blank'))));
  }
  $old_dir = $wrapper->getDirectoryPath() . '/' . config_get('contest.settings', 'export_dir');
  _contest_purge_dir("$old_dir/*", 0);
  @chmod($old_dir, 0777);
  rmdir($old_dir);

  _contest_create_export_directory($path);

  $cfg->set('export_dir', $path);

  $cfg->save();
}
/**
 * Custom validation function for the contest setting form.
 */
function contest_settings_form_validate($form, &$form_state) {

// Directory name doesn't match requirements.

  if (!preg_match('/^\w[\w\/]*\w$/', $form_state['values']['export_dir'])) {
    form_set_error('export_dir', t('The export directory can contain alpha-numeric characters, hyphens and slashes and must start and end with a alpha-numeric character.'));
  }
  if ($form_state['values']['export_dir'] !== config_get('contest.settings', 'export_dir')) {
    $wrapper = file_stream_wrapper_get_instance_by_uri('private://');

// Private system path not configured.

    if (!$wrapper) {
      form_set_error('export_dir', t("Backdrop's file system path needs to be configured. Please go to !link to configure.", array('!link' => l('File System', 'admin/config/media/file-system', array('target' => '_blank')))));
    }
    $path = $wrapper->getDirectoryPath() . '/' . _contest_clean_path($form_state['values']['export_dir']);

// Directory already in use.

    if (is_dir($path) || is_file($path)) {
      form_set_error('export_dir', t('That export directory is already in use.'));
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
