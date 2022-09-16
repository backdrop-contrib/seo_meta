SEO Meta Tags
===================================

Provide "meta tags" and customizable "title" tag for your site's pages.
Search engine optimization assumed using an essential set of meta tags to help improve search ranking for your site.
Meta tags may be used to control the display of content across popular social networks, such as Facebook or Twitter.

Please note
-----------
1. This module is intended to replace module [Base Meta: Page Title And Meta Tags] (https://backdropcms.org/project/base_meta) as refactored and extended successor, without backward compatibility.
    
2. This module is NOT intended to completely replace or repeat all the functions of its "big brother" ("Metatag" module) - only the necessary minimum set of functions.

Features
--------

Pages on your site may be created by adding node, views pages or just taxonomy term.
For any page created this way, you may add:
  
- customized "title" tag instead generated by system;
- site name: may be appended or removed for "title" tag with customizable divider;
- "descriptions", "keywords" (optionally) and "robots" meta tags; 
- Facebook Open Graph Protocol meta tags, specifically: "og:url", "og:type", "og:title", "og:description", "og:site_name" and "og:image";
- Twitter Cards meta tags, specifically: "twitter:card", "twitter:site" and "twitter:creator".  

All tags may be set up for each page/node specified, or as set of defaults for all pages where meta tags is not exactly defined.

Another "canonical URL" may be defined instead of default for any page.

Different "base URL" may be used in all canonical URLs instead of default 
(or actual) site domain. May be useful for mirroring.

For site's "shortcut icon" link you may change "rel" and "type" attributes.

Installation
------------

Install this module using the official Backdrop CMS instructions at
 https://backdropcms.org/guide/modules

Grant permissions on the "Permissions" page (Administration > Configuration > User accounts > Permissions 
or config/people/permissions) - "Administer SEO Meta Tags settings" to the roles that are allowed to control meta tags on the site.

Configuration and usage
-----------------------
The main configuration page (Administration > Configuration > 
Metadata > SEO Meta tags or admin/config/metadata/seo_meta) 
control defaults and may be used for:

- set up default values for meta tags;
- enable or disable usage of site name in page titles and set divider in between;
- define replacement for "Canonical Base URL" (see description on the form); 
- enable availability for obsolete "Keywords" meta tag;
- change shortcut icon link "rel" and "type" attributes (see description on the form);

Meta tags for "Front page" (Home page) may be set by using tab 
in admin area: (Administration > Configuration > Metadata > 
SEO Meta tags > Front page or admin/config/metadata/seo_meta/home). 
If more than one language available, links to "Front page meta tags edit form" 
for each of languages will be provided.

Meta tags for pages provided by taxonomy tags pages may be set by using tab 
in admin area: (Administration > Configuration > Metadata > 
SEO Meta tags > Taxonomy pages or admin/config/metadata/seo_meta/taxonomy).  
Select appropriate vocabulary, then select any available 
term name to open "Taxonomy page meta tags edit form" for this term page;
 
Meta tags for pages provided by module Views" may be set by using tab 
in admin area: (Administration > Configuration > Metadata > 
SEO Meta tags > Views or admin/config/metadata/seo_meta/views) 
and select any available page name to open "Views page meta tags edit form".

**For any page generated by node**: on node edit form you may use vertical tab named 'SEO meta tags' - select checkbox "Define tags for page of this node" and fill in the appropriate fields to override defaults.

**Screenshots** are available at https://findlab.net/projects/seo-meta

License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.

Current Maintainer
------------------

Vladimir (https://github.com/findlabnet/)

For bug reports, feature or support requests, please use the module 
issue queue at https://github.com/backdrop-contrib/seo_meta/issues.

Credits
------------------
Tested with the valuable help of **bd0bd** (https://github.com/bd0bd)
