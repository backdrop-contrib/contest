Module: Contest
Author: bkelly bkelly@265918.no-reply.drupal.org

Description
===========
Allows your site to host contests on a website, (leagally I think they're actually "Sweepstakes" but I'm no lawyer).

Requirements
============
Requires the path profile profile_location modules.

Installation
============
1. Copy the 'contest' directory in to your Drupal modules directory.

2. Copy the contest/node-contest.tpl.php file to your theme's template directory and edit as you see fit.

3. Enable the module.

4. Flush the cache.

5. Go to admin/settings/contest and configure the contest settings.

6. Complete a profile for the contest host, (defaults to user 1). This information is used in the contest view, (particularly the rules fieldset).

7. Complete a profile for the contest sponsor, (if host and sponsor are the same you can skip this). Again, this information is used in the contest view, (particularly the rules fieldset).
