Module: Contest
Author: bkelly bkelly@265918.no-reply.drupal.org

Description
===========
Allows your site to host a sweepstakes, (contestants win prizes via a random drawing).

Requirements
============
Requires the path profile profile_location modules.

Installation
============
1. Copy the 'contest' directory in to your Drupal modules directory.

2. Go to /admin/config/regional/settings and configure the country and time zone.

3. Go to /admin/config/media/file-system and configure the private file system path.

4. Enable the module.

5. Flush the cache.

3. Go to /admin/settings/contest and configure the contest settings.

6. Complete a profile for the contest host, (defaults to user 1). This information 
   is used in the contest view, (particularly the rules fieldset).

7. Complete a profile for the contest sponsor, (if host and sponsor are the same 
   you can skip this). Again, this information is used in the contest view,
   (particularly the rules fieldset).
