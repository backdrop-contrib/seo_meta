<?php

/**
 * @file
 * Admin forms to manage meta tags
 *
 */

/**
 * 
 * Administration form for default seo meta tags.
 *
 */
function seo_meta_settings_form() {
  
  global $language;
  $config      = config('seo_meta.settings');
  $core_config = config('system.core');
  $saved_tags  = seo_meta_get_saved_tags(TYPE_DEFAULTS, $language->langcode);
  
  $form = array();
  
  $form['langcode'] = array(
    '#type' => 'hidden',
    '#value' => $language->langcode,
  );
  
  $form['#attached']['css'] = array(
    backdrop_get_path('module', 'seo_meta') . '/css/seo_meta.css',
  );

  $form['description'] = array(
    '#prefix' => '<div>',
    '#markup' => 'This module provides an interface for adding meta tags to pages.'
    . '<br />Values of meta tags defined below will be added as default to pages where meta tags was not specified.'
    . '<br />Defaults can be overrided:'
    . '<br />&nbsp;- for any node page - using vertical tab "SEO meta tags" on node edit form;</em>'
    . '<br />&nbsp;- for views, taxonomy or home pages - using corresponding tabs of this form.',
    '#suffix' => '</div>',
  );
  
  $form['basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic meta tags'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['basic']['base_canonical_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Base Canonical URL'),
    '#default_value' => $config->get('base_canonical_url'),
    '#size' => 70,
    '#maxlength' => 255,
    '#description' => t('This is replacement for "Base URL" for any canonical URL on this site. May be useful for mirroring.')
      .'<br />'
      . t('Please insert domain without protocol, for example: www.example.com'),
  );
  $form['basic']['use_keywords'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use keywords meta tag.'),
    '#default_value' => $config->get('use_keywords'),
    '#description' => t('If checked - field for keywords meta will be enabled sitewide. Google does not use the keywords meta tag, but if you want to use them, use them.'),
  );
  $form['basic']['meta_keywords'] = array(
    '#type' => 'textfield',
    '#title' => t('Default meta keywords'),
    '#default_value' => isset($saved_tags['meta_keywords']) ? $saved_tags['meta_keywords'] : '',
    '#size' => 70,
    '#maxlength' => 255,
    '#states' => array(
    // Hide the settings when using keywords checkbox is disabled.
      'invisible' => array(
      ':input[name="use_keywords"]' => array('checked' => FALSE),
      ),
    ),
    '#description' => t('Comma separated list of keywords – no more than 10 keyword phrases.'),
  );
  $form['basic']['meta_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Default meta description'),
    '#default_value' => isset($saved_tags['meta_description']) ? $saved_tags['meta_description'] : '',
    '#rows' => 3,
    '#cols' => 30,
    '#description' => t('One or two sentence described this site (160-200 characters max. without any HTML markup.).'),
  );
  $form['basic']['meta_robots'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Default meta robots'),
    '#options' => backdrop_map_assoc(array('NOINDEX', 'NOFOLLOW', 'NOARCHIVE', 'NOODP', 'NOYDIR', 'NOSNIPPET')),
    '#default_value' => isset($saved_tags['meta_robots']) ? unserialize($saved_tags['meta_robots']) : array(),
    '#description' => 'Search engines assume "INDEX" and "FOLLOW" unless these are specifically disabled above.',
  );
  $form['basic']['fix_favicon'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fix shortcut icon link "rel" and "type" attributes'),
    '#default_value' => $config->get('fix_favicon'),
    '#description' => t('If this option is selected, a link to a site icon will change, as in the example below, from:') . 
    '<br />' . '&lt;link rel="shortcut icon" href="http://example.com/core/misc/favicon.ico" type="image/vnd.microsoft.icon" /&gt;'.
    '<br />' . t('to:') .
    '<br />' . '&lt;link rel="icon" href="http://example.com/core/misc/favicon.ico" type="image/x-icon" /&gt;',
  );
  $form['basic']['append_site_name'] = array(
    '#type' => 'checkbox',
    '#title' => t('Append site name to page title'),
    '#default_value' => $config->get('append_site_name'),
    '#description' => t('Append the site name to the page title. This site name: "@site_name".', array('@site_name' => $core_config->get('site_name'))),
  );
  $form['basic']['site_name_divider'] = array(
    '#type' => 'textfield',
    '#title' => t('Site name divider'),
    '#default_value' => $config->get('site_name_divider'),
    '#size' => 10,
    '#maxlength' => 10,
    '#description' => t('If the site name is being append, value of this field will be displayed between the page title and the site name.'),
    '#element_validate' => array('seo_meta_site_name_divider_validate'),
  );
  
  $form['open_graph'] = array(
    '#type' => 'fieldset',
    '#title' => t('Open Graph'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['open_graph']['meta_og_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Open Graph title'),
    '#default_value' => isset($saved_tags['meta_og_title']) ? $saved_tags['meta_og_title'] : '',
    '#description' => t('The title of this page/article as rendered on Facebook (without any branding such as your site name.)'),
  );
  $form['open_graph']['meta_og_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Open Graph description'),
    '#default_value' => isset($saved_tags['meta_og_description']) ? $saved_tags['meta_og_description'] : '',
    '#rows' => 3,
    '#description' => t('A brief description of the content, usually between 2 and 4 sentences (without HTML markup). This will displayed below the title of the post on Facebook.'),
  );
  $form['open_graph']['meta_og_type'] = array(
    '#title' => t('Open Graph type'),
    '#type' => 'select',
    '#options' => array(
      '' => t('Select type'),
      'website' => 'website',
      'article' => 'article',
      'product' => 'product',
      'profile' => 'profile',
    ),
    '#default_value' => isset($saved_tags['meta_og_type']) ? $saved_tags['meta_og_type'] : '',
    '#description' => t('The type of media of your content.'),
  );
  
  $form['open_graph']['meta_og_image_fid'] = array(
    '#title' => t('Upload image that appears when someone shares the content to Facebook.'),
    '#type' => 'managed_file',
    '#upload_location' => 'public://' . $config->get('meta_og_images_dir'),
    '#default_value' => isset($saved_tags['meta_og_image_fid']) ? $saved_tags['meta_og_image_fid'] : '',
    '#upload_validators' => array(
      'file_validate_extensions' => array('gif png jpg jpeg'),
      'file_validate_size' => array(file_upload_max_size()),
    ),
  );
  $form['open_graph']['meta_og_site_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Site name'),
    '#default_value' => isset($saved_tags['meta_og_site_name']) ? $saved_tags['meta_og_site_name'] : '',
    '#description' => t('The name of your website (such as IMDb, not imdb.com).'),
  );

  $form['twitter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Twitter'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['twitter']['meta_tw_card'] = array(
    '#title' => t('Twitter Card type'),
    '#type' => 'select',
    '#options' => array(
      '' => t('Select card type'),
      'summary' => 'Summary Card',
      'summary_large_image' => 'Summary Card with Large Image',
      'photo' => 'Photo Card',
      'gallery' => 'Gallery Card',
      'product' => 'Product Card',
    ),
    '#default_value' => isset($saved_tags['meta_tw_card']) ? $saved_tags['meta_tw_card'] : '',
    '#description' => t('The card type.'),
  );
  $form['twitter']['meta_tw_site'] = array(
    '#type' => 'textfield',
    '#title' => t('Site'),
    '#default_value' => isset($saved_tags['meta_tw_site']) ? $saved_tags['meta_tw_site'] : '',
    '#description' => t('@username of website.'),
  );
  $form['twitter']['meta_tw_creator'] = array(
    '#type' => 'textfield',
    '#title' => t('Creator'),
    '#default_value' => isset($saved_tags['meta_tw_creator']) ? $saved_tags['meta_tw_creator'] : '',
    '#description' => t('@username of content creator.'),
  );
  $form['twitter']['div'] = array(
    '#markup' => '<div class="description">' . t('Note: When the Twitter card processor looks for tags on your page, 
       it first checks for the Twitter property, and if not present, 
       falls back to the supported Open Graph property (for example site URL, title, description and image). 
       This allows for both to be defined on the page independently, and minimizes 
       the amount of duplicate markup required to describe your content and experience.') . '</div>',
  );
  
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  return $form;
}

/**
 * Implements element_validate().
 * 
 */
function seo_meta_site_name_divider_validate($element, &$form_state, $form) {
  $append_site_name = $form_state['values']['append_site_name'];
  
  if (empty($element['#value']) && $append_site_name) {
    form_error($element, t('Field "Site name divider" cannot be empty when option "Append site name to page title" is selected.'));
  }
}

/**
 * Implements hook_submit().
 * 
 */
function seo_meta_settings_form_submit($form, &$form_state) {
  
  global $user;
  $config      = config('seo_meta.settings');
  $langcode    = $form_state['values']['langcode'];
  $saved_tags  = seo_meta_get_saved_tags(TYPE_DEFAULTS, $langcode);
  
  $config->set('base_canonical_url', trim($form_state['values']['base_canonical_url']));
  $config->set('use_keywords',       $form_state['values']['use_keywords']);
  $config->set('fix_favicon',        $form_state['values']['fix_favicon']);
  $config->set('append_site_name',   $form_state['values']['append_site_name']);
  $config->set('site_name_divider',  trim($form_state['values']['site_name_divider']));
  
  $config->save();
  
  $keywords = $form_state['values']['use_keywords'] ? trim($form_state['values']['meta_keywords']) : '';
  
  $data = array (
    'ce_type'             => TYPE_DEFAULTS,
    'ce_id'               => $langcode, // $langcode used as id for defaults and homepages
    'path_alias'          => 'und',
    'langcode'	          => $langcode,
    'page_title'          => 'und',
    'meta_keywords'       => $keywords,
    'meta_description'    => trim($form_state['values']['meta_description']),
    'meta_robots'         => serialize($form_state['values']['meta_robots']),
    'rel_canonical'       => 'und',
    'meta_og_title'       => trim($form_state['values']['meta_og_title']),
    'meta_og_description' => trim($form_state['values']['meta_og_description']),
    'meta_og_type'        => $form_state['values']['meta_og_type'],
    'meta_og_image_fid'   => $form_state['values']['meta_og_image_fid'],
    'meta_og_site_name'   => trim($form_state['values']['meta_og_site_name']),
    'meta_tw_card'        => $form_state['values']['meta_tw_card'],
    'meta_tw_site'        => trim($form_state['values']['meta_tw_site']),
    'meta_tw_creator'     => trim($form_state['values']['meta_tw_creator']),
    'uid'                 => $user->uid,   
    'changed'             => time(),
  );
  
  if (empty($saved_tags)) {
    seo_meta_record_insert($data);
  }
  else {
    seo_meta_record_update(TYPE_DEFAULTS, $langcode, $data);
  }
  
  if (!empty($form_state['values']['meta_og_image_fid'])) {
    $file = file_load($form_state['values']['meta_og_image_fid']);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    file_usage_add($file, 'seo_meta', 'user', $user->uid); 
  }
  
  backdrop_set_message(t('The configuration options have been saved.'));
} 

/**
 * Form wrapper for homepage
 */
function seo_meta_settings_homepage_form($form, &$form_state) {
  
  // We work with enabled languages only
  $site_langs = language_list(TRUE);
  
  if (count($site_langs) == 1) {
    
    return seo_meta_custom_item_tags_edit();
  }
  else {
    $form['#attached']['css'] = array(
      backdrop_get_path('module', 'seo_meta') . '/css/seo_meta.css',
    );

    $form['description'] = array(
      '#markup' => t('This site have more than one content language enabled, '
          . 'so you can define meta tags for homepages on each of them.'),
      '#weight' => -12,
    );
   
    foreach ($site_langs as $lang) {
      $langcode = $lang->langcode;
      $site_default_lang = $lang->default ? t('(site default language)') : '';
      $form_path = 'admin/config/search/seo_meta/home/' . $langcode;
      
      $form[$langcode . '_homepage'] =array(
        '#markup' => '<p>' . l(t('Meta tags for @lang @def homepage.', array('@lang' => $lang->name, '@def' => $site_default_lang)), $form_path, array()) . '</p>',
        '#weight' => $lang->default ? -11 : $lang->weight,
      );
    }

    return $form;
  }
}

/**
 * Form wrapper for views pages
 */
function seo_meta_settings_views_form($form, &$form_state) {

  $views_pages = seo_meta_get_views_pages();

  $form['description'] = array(
    '#markup' => 'List of <em>views pages</em> for which you can define page title and meta tags.',
  );

  $form['views_fieldset'] = array(
    '#title' => t("Views pages available"),
    '#type' => 'fieldset',
  );
  
  if (!empty($views_pages)) {
    foreach ($views_pages as $page) {
      foreach ($page as $view_name => $page_path) {
        $form['views_fieldset']['test_' . $view_name] = array(
          
          '#markup' => l($view_name, 'admin/config/search/seo_meta/views/' . $view_name, array('attributes' => array('title' => t('Click to edit meta tags for this page.')))). '<br />',
        );
      }
    }
  }
  else {
    $form['views_fieldset']['empty'] = array(
      '#markup' => t('You have not non-admin views with display of type "Page"'),
    );
  }
  
  return $form;
}

/**
 * Form wrapper for taxonomy pages
 */
function seo_meta_settings_taxonomy_form($form, &$form_state) {

  $vocabularies = array('-' => t('None selected'));
  $vocs = taxonomy_get_vocabularies();
  foreach ($vocs as $m_name => $v_obj) {
    $vocabularies[$m_name] = $v_obj->name;
  }
  
  $form['description'] = array(
    '#markup' => 'List of <em>taxonomy pages</em> for which you can define page title and meta tags.',
  );
  
  $selected = !empty($form_state['values']['terms_select']) ? $form_state['values']['terms_select'] : key($vocabularies);

  $form['terms_select'] = array(
    '#title' => t('Select vocabulary name'),
    '#type' => 'select',
    '#options' => $vocabularies,
    '#default_value' => $selected,
    '#ajax' => array(
      'callback' => 'seo_meta_taxonomy_callback',
      'wrapper' => 'terms-div',
      'method' => 'replaceWith',
      'effect' => 'slide',
      'progress' => array('type' => 'none'),
    ),
  );

  $form['terms_fieldset'] = array(
    '#title' => t("Terms Available"),
    '#prefix' => '<div id="terms-div">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
    '#states' => array(
      // Hide the fieldset when no vocabulary selected.
      'invisible' => array(
       ':input[name="terms_select"]' => array('value' => '-'),
      ),
    ),
  );

  if ($selected != '-') {
    
    $terms = taxonomy_get_tree($selected);

    if (!empty($terms)) {
      
      foreach ($terms as $key => $term_obj) {
        
        $form['terms_fieldset'][$key][$selected] = array(

          '#markup' => l($term_obj->name, 'admin/config/search/seo_meta/taxonomy/' . $term_obj->name, array('attributes' => array('title' => t('Click to edit meta tags for this page.')))). '<br />',
        );
      }
    }
    else {
      
      $form['terms_fieldset'][$selected] = array(
        '#title' => '',
        '#markup' => t('No terms available.'),
      );
    }
  }

  return $form;
}

/**
 * Callback for taxonomy.
 *
 * @return array Renderable array (the taxonomy fieldset)
 */
function seo_meta_taxonomy_callback($form, $form_state) {
  
  return $form['terms_fieldset'];
}

/**
 * Edit form for views, taxonomy and home pages 
 * 
 * @param type $form
 * @param type $form_state
 */
function seo_meta_custom_item_tags_edit($form, &$form_state) {

  $config       = config('seo_meta.settings');
  $use_keywords = $config->get('use_keywords');

  $args         = arg();
  $page_type    = $args[4];
  
  if ($page_type == 'home') { 
    $page_name     = '';
    $page_langcode = $args[5];
    $type          = TYPE_HOMEPAGE;
    $page_id       = $page_langcode;
    
    $site_langs = language_list(TRUE);
    if (!array_key_exists($page_langcode, $site_langs)) {
      form_set_error('', t('Language with code @langcode do not enabled for this site!', array('@langcode' => '"' . $page_langcode . '"')));
      return;
    }    
    
    $subtitle = t('For <em>' . language_name($page_langcode) . '</em> homepage');
  }
  else{
    $page_name = $args[5];
    $type      = $page_type == 'views' ? TYPE_VIEW : TYPE_TAXONOMY;
    $page_id   = $page_name;
    $subtitle  = t('For ' . $page_type . ' page named <em>' . $page_name . '</em>');
  }
  
  $saved_tags = seo_meta_get_saved_tags($type, $page_id);
  
  $form['page_info']['page_type'] = array(
    '#type' => 'hidden',
    '#value' => $type,
  );  
  $form['page_info']['page_id'] = array(
    '#type' => 'hidden',
    '#value' => $page_id,
  );  
  $form['page_info']['with_tags'] = array(
    '#type' => 'hidden',
    '#value' => is_array($saved_tags) ? TRUE : FALSE,
  );  
  $form['page_info']['use_keywords'] = array(
    '#type' => 'hidden',
    '#value' => $use_keywords,
  );  
  
  $form['#attached']['css'] = array(
    backdrop_get_path('module', 'seo_meta') . '/css/seo_meta.css',
  );
  
  $form['seo_meta'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page title and meta tags'),
    '#access' => user_access('edit base meta tags'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['seo_meta']['define'] = array(
    '#markup' => $subtitle,
  );

  $form['seo_meta']['page_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Page title'),
    '#default_value' => isset($saved_tags['page_title']) ? $saved_tags['page_title'] : '',
    '#description' => t('Description of this page to appear in the &lt;title&gt; tag for search engines search result listings.'),
  );

  if ($use_keywords) {
    $form['seo_meta']['meta_keywords'] = array(
      '#type' => 'textfield',
      '#title' => t('Meta keywords'),
      '#default_value' => isset($saved_tags['meta_keywords']) ? $saved_tags['meta_keywords'] : '',
      '#size' => 120,
      '#maxlength' => 255,
      '#description' => t('Comma separated list of keywords (no more than 10 keyword phrases).'),
    );
  }
  $form['seo_meta']['meta_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Meta description'),
    '#default_value' => isset($saved_tags['meta_description']) ? $saved_tags['meta_description'] : '',
    '#rows' => 3,
    '#description' => t('One or two sentence described this page (160-200 characters max. without any HTML markup.)'),
  );

  $form['seo_meta']['meta_robots'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Meta robots'),
    '#options' => backdrop_map_assoc(array('NOINDEX', 'NOFOLLOW', 'NOARCHIVE', 'NOODP', 'NOYDIR', 'NOSNIPPET')),
    '#default_value' => isset($saved_tags['meta_robots']) ? unserialize($saved_tags['meta_robots']) : array(),
    '#description' => 'Search engines assume "INDEX" and "FOLLOW" unless these are specifically disabled above.',
  );
  $form['seo_meta']['rel_canonical'] = array(
    '#type' => 'textfield',
    '#title' => t('Canonical URL'),
    '#default_value' => isset($saved_tags['rel_canonical']) ? $saved_tags['rel_canonical'] : '',
    '#description' => t("Use a relative URL without the initial slash for current default domain or full URL."
      . " By default (if no value) this will automatically use URL of this page."),
  );
  
  $form['seo_meta']['open_graph'] = array(
    '#type' => 'fieldset',
    '#title' => t('Open Graph'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['seo_meta']['open_graph']['meta_og_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Open Graph title'),
    '#default_value' => isset($saved_tags['meta_og_title']) ? $saved_tags['meta_og_title'] : '',
    '#description' => t('The title of this page/article as rendered on Facebook (without any branding such as your site name.)'),
  );
  $form['seo_meta']['open_graph']['meta_og_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Open Graph description'),
    '#default_value' => isset($saved_tags['meta_og_description']) ? $saved_tags['meta_og_description'] : '',
    '#rows' => 3,
    '#description' => t('A brief description of the content, usually between 2 and 4 sentences (without HTML markup). This will displayed below the title of the post on Facebook.'),
  );
  $form['seo_meta']['open_graph']['meta_og_type'] = array(
    '#title' => t('Open Graph type'),
    '#type' => 'select',
    '#options' => array(
      '' => t('Select type'),
      'website' => 'website',
      'article' => 'article',
      'product' => 'product',
      'profile' => 'profile',
    ),
    '#default_value' => isset($saved_tags['meta_og_type']) ? $saved_tags['meta_og_type'] : '',
    '#description' => t('The type of media of your content.'),
  );
  
  $form['seo_meta']['open_graph']['meta_og_image_fid'] = array(
    '#title' => t('Upload image that appears when someone shares the content to Facebook.'),
    '#type' => 'managed_file',
    '#upload_location' => 'public://' . $config->get('meta_og_images_dir'),
    '#default_value' => isset($saved_tags['meta_og_image_fid']) ? $saved_tags['meta_og_image_fid'] : '',
    '#upload_validators' => array(
      'file_validate_extensions' => array('gif png jpg jpeg'),
      'file_validate_size' => array(file_upload_max_size()),
    ),
  );
  $form['seo_meta']['open_graph']['meta_og_site_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Site name'),
    '#default_value' => isset($saved_tags['meta_og_title']) ? $saved_tags['meta_og_title'] : '',
    '#description' => t('The name of your website (such as IMDb, not imdb.com).'),
  );

  $form['seo_meta']['twitter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Twitter'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['seo_meta']['twitter']['meta_tw_card'] = array(
    '#title' => t('Twitter Card type'),
    '#type' => 'select',
    '#options' => array(
      '' => t('Select card type'),
      'summary' => 'Summary Card',
      'summary_large_image' => 'Summary Card with Large Image',
      'photo' => 'Photo Card',
      'gallery' => 'Gallery Card',
      'product' => 'Product Card',
    ),
    '#default_value' => isset($saved_tags['meta_tw_card']) ? $saved_tags['meta_tw_card'] : '',
    '#description' => t('The card type.'),
  );
  $form['seo_meta']['twitter']['meta_tw_site'] = array(
    '#type' => 'textfield',
    '#title' => t('Site'),
    '#default_value' => isset($saved_tags['meta_tw_site']) ? $saved_tags['meta_tw_site'] : '',
    '#description' => t('@username of website.'),
  );
  $form['seo_meta']['twitter']['meta_tw_creator'] = array(
    '#type' => 'textfield',
    '#title' => t('Creator'),
    '#default_value' => isset($saved_tags['meta_tw_creator']) ? $saved_tags['meta_tw_creator'] : '',
    '#description' => t('@username of content creator.'),
  );
  $form['seo_meta']['twitter']['div'] = array(
    '#markup' => '<div class="description">' . t('Note: When the Twitter card processor looks for tags on your page, 
       it first checks for the Twitter property, and if not present, 
       falls back to the supported Open Graph property (for example site URL, title, description and image). 
       This allows for both to be defined on the page independently, and minimizes 
       the amount of duplicate markup required to describe your content and experience.') . '</div>',
  );

  $form['seo_meta']['actions'] = array(
    '#type' => 'actions'
  );
  $form['seo_meta']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Implements hook_validate().
 * Just check if at least one field from basics is not empty. 
 */
function seo_meta_custom_item_tags_edit_validate($form, &$form_state) {
  $seo_meta_values = $form_state['values'];

  $empty_tags = (
      empty($seo_meta_values['page_title']) &&
      empty($seo_meta_values['meta_keywords']) &&
      empty($seo_meta_values['meta_description']) &&
      empty($seo_meta_values['rel_canonical'])) ? TRUE : FALSE;

  $checked_boxes = FALSE;

  foreach ($seo_meta_values['meta_robots'] as $tag => $value) {
    if (is_string($value)) {
      $checked_boxes = TRUE;
    }
  }

  if ($empty_tags && !$checked_boxes) {
    form_set_error('', t('You must fill some values to the fields, nothing to save!'));
  }
}

/**
 * Implements hook_submit().
 * 
 */
function seo_meta_custom_item_tags_edit_submit($form, &$form_state) {

  global $user, $language; 

  $page_type       = $form_state['input']['page_type'];
  $page_id         = $form_state['input']['page_id'];
  $with_tags       = $form_state['input']['with_tags'];
  $use_keywords    = $form_state['input']['use_keywords'];
  $keywords        = $use_keywords ? trim($form_state['values']['meta_keywords']) : '';
  
  $data = array (
    'ce_type'             => $page_type,
    'ce_id'               => $page_id,
    'path_alias'          => 'fix-it-later',
    'langcode'	          => $language->langcode,
    'page_title'          => trim($form_state['values']['page_title']),
    'meta_keywords'       => $keywords,
    'meta_description'    => trim($form_state['values']['meta_description']),
    'meta_robots'         => serialize($form_state['values']['meta_robots']),
    'rel_canonical'       => trim($form_state['values']['rel_canonical']),
    'meta_og_title'       => trim($form_state['values']['meta_og_title']),
    'meta_og_description' => trim($form_state['values']['meta_og_description']),
    'meta_og_type'        => $form_state['values']['meta_og_type'],
    'meta_og_image_fid'   => $form_state['values']['meta_og_image_fid'],
    'meta_og_site_name'   => trim($form_state['values']['meta_og_site_name']),
    'meta_tw_card'        => $form_state['values']['meta_tw_card'],
    'meta_tw_site'        => trim($form_state['values']['meta_tw_site']),
    'meta_tw_creator'     => trim($form_state['values']['meta_tw_creator']),
    'uid'                 => $user->uid,   
    'changed'             => time(),
  );
  
  if ($with_tags) {
    seo_meta_record_update($page_type, $page_id, $data);
  }
  else {
    seo_meta_record_insert($data);
  }
  
  if (!empty($form_state['values']['meta_og_image_fid'])) {
    $file = file_load($form_state['values']['meta_og_image_fid']);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    file_usage_add($file, 'seo_meta', 'user', $user->uid); 
  }
  
  backdrop_set_message(t('Tags for page have been saved.'));
}

/**
 * 
 * @return array of views pages with path defined
 */
function seo_meta_get_views_pages() {
  
  $result = array();
  $views = views_get_enabled_views();
  foreach ($views as $view) {

    if (empty($view->display)) {
      // Skip this view as it is broken.
      debug(t("Skipping broken view @view", array('@view' => $view->name)));
      continue;
    }
    
    foreach (array_keys($view->display) as $id) {
      $plugin = views_fetch_plugin_data('display', $view->display[$id]->display_plugin);

      if ($plugin['name'] == 'page') {
        $view_name = $view->name;
        $page_path = $view->display[$id]->display_options['path'];
        // Skip admin views
        if (strpos($page_path, 'admin/') === FALSE) {
          $result[][$view_name] = $page_path;
        }
      }           
    }
  }
  
  return $result;
}


