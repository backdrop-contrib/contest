<div id="contest-admin">
  <fieldset class="contest-admin-host">
    <legend>Host</legend>
    <?php print l($data->host->title, "user/{$data->host->uid}"); ?><br />
    <?php print l($data->host->mail, "mailto:{$data->host->mail}", array('absolute' => TRUE)); ?><br />
    <?php print t('Phone: @phone', array('@phone' => $data->host->phone)); ?><br />
    <?php print t('Address: @address', array('@address' => "{$data->host->address}, {$data->host->city} {$data->host->state} {$data->host->zip}")); ?>
  </fieldset>
  <fieldset class="contest-admin-sponsor">
    <legend>Sponsor</legend>
    <?php print l($data->contest->sponsor->name, "user/{$data->contest->sponsor->uid}"); ?><br />
    <?php print l($data->contest->sponsor->mail, "mailto:{$data->contest->sponsor->mail}", array('absolute' => TRUE)); ?><br />
    <?php print l(preg_replace('/https?:\/\//', '', $data->contest->sponsor->url), $data->contest->sponsor->url, array('absolute' => TRUE)); ?>
  </fieldset>
  <div class="contest-admin-detail">
    <?php print t('Start Date: !start_date', array('!start_date' => format_date($data->contest->start, 'long'))); ?><br />
    <?php print t('End Date: !end_date', array('!end_date' => format_date($data->contest->end, 'long'))); ?><br />
    <?php print t('Total Entries: !entries', array('!entries' => $data->contest->entries)); ?><br />
    <?php print t('Total Users: !entrants', array('!entrants' => $data->contest->entrants)); ?><br />
    <?php print t('Places Allowed: !places', array('!places' => $data->contest->places)); ?>
  </div>
</div>
<pre>
  <?php print_r($data); ?>
</pre>