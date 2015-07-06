<div id="contest-admin">
  <fieldset class="contest-admin-host">
    <legend>Host</legend>
    <?php print l($data->host->title, "user/{$data->host->uid}"); ?><br />
    <?php print l($data->host->mail, "mailto:{$data->host->mail}", array('absolute' => TRUE)); ?><br />
    <?php print t('Phone: @phone', array('@phone' => _contest_get_usr_val($data->host, 'field_contest_phone'))); ?><br />
    <?php print t('Address: @address', array('@address' => $data->host->address)); ?>
  </fieldset>
  <fieldset class="contest-admin-sponsor">
    <legend>Sponsor</legend>
    <?php print l($data->contest->sponsor, "user/{$data->contest->sponsor_uid}"); ?><br />
    <?php print l($data->contest->sponsor_email, "mailto:{$data->contest->sponsor_email}", array('absolute' => TRUE)); ?><br />
    <?php print l(preg_replace('/https?:\/\//', '', $data->contest->sponsor_url), $data->contest->sponsor_url, array('absolute' => TRUE)); ?>
  </fieldset>
  <div class="contest-admin-detail">
    <?php print t('Start Date: !start_date', array('!start_date' => format_date($data->contest->start, 'long'))); ?><br />
    <?php print t('End Date: !end_date', array('!end_date' => format_date($data->contest->end, 'long'))); ?><br />
    <?php #print t('', array('!' => $data->contest->start)); ?>
<!--
    Total Entries: 2
    Total Users: 2
--->    
    <?php print t('Places Allowed: !places', array('!places' => $data->contest->places)); ?><br />
  </div>
</div>
<pre>
  <?php print_r($data); ?>
</pre>