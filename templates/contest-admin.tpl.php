<?php

/**
 * @file contest-admin.tpl.php
 * Template for a contest's admin page.
 *
 * Available variables:
 * - $data (object)
 * - - node (object) contest node object
 * - - contest (object)
 * - - - start (int)
 * - - - end (int)
 * - - - places (int)
 * - - - period (int)
 * - - - publish_winners (bool)
 * - - - entrants (int)
 * - - - entries (int)
 * - - contestants (array of objects) index => contestant object
 * - - - uid (int)
 * - - - name (string)
 * - - - mail (string)
 * - - - qty (int)
 * - - - created (int)
 * - - - winner (bool)
 * - - host (object)
 * - - - uid (int)
 * - - - name (string)
 * - - - mail (string)
 * - - - title (string)
 * - - - full_name (string)
 * - - - business (string)
 * - - - address (string)
 * - - - city (string)
 * - - - state (string)
 * - - - zip (string)
 * - - - phone (string)
 * - - - birthdate (int)
 * - - sponsor (object)
 * - - - uid (int)
 * - - - name (string)
 * - - - mail (string)
 * - - - url (string)
 * - - - full_name (string)
 * - - - business (string)
 * - - - address (string)
 * - - - city (string)
 * - - - state (string)
 * - - - zip (string)
 * - - - phone (string)
 * - - - birthdate (int)
 * - - winners (array) uid => place
 */
?>
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
  
<?php if ($data->contest->end > REQUEST_TIME): ?>
  <ul class="contest-admin-actions">
    <li><?php print l(t('Pick Random Winner'), "contest/pick-winner/{$data->node->nid}"); ?></li>
    <li><?php print $data->contest->publish_winners? l(t('Unpublish Winners'), "contest/unpublish-winners"):  l(t('Publish Winners'), "contest/publish-winners"); ?></li>
    <li><?php print l(t('Clear All Winners'), "contest/clear-winners/{$data->node->nid}"); ?></li>
    <li><?php print l(t('Export Entries'), "contest/export-entries/{$data->node->nid}"); ?></li>
    <li><?php print l(t('Export Unique Users'), "contest/export-entrants/{$data->node->nid}"); ?></li>
  </ul>
<?php endif; ?>
  
  <table border="0" cellspacing="0" cellpadding="3" class="contest-admin-contestants">
    <caption>Contest Entrants</caption>
    <thead>
      <tr>
        <th><?php print t('Name'); ?></th>
        <th><?php print t('Email'); ?></th>
        <th><?php print t('Count'); ?></th>
        <th><?php print t('Winner'); ?></th>
      </tr>    
    </thead>
    <tbody>
      
    <?php foreach ($data->contestants as $usr): ?>
      <tr>
        <td><?php print l($usr->name, "user/$usr->uid"); ?></td>
        <td><?php print l($usr->mail, "mailto:$usr->mail", array('absolute' => TRUE)); ?></td>
        <td><?php print $usr->qty; ?></td>
        <td><?php print $usr->winner? l(t('Clear'), 'contest/clear-winner'): l(t('Pick'), "contest/pick-winner/{$data->node->nid}/$usr->uid"); ?></td>
      </tr>
    <?php endforeach; ?>
    
    </tbody>
  </table>
</div>
<pre>
  <?php print_r($data); ?>
</pre>