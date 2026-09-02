<?php

/**
 * @file
 * Administration forms to manage meta tags
 *
 */

/**
 * Returns the supported robots form options.
 */
function seo_meta_robots_options() {
  return backdrop_map_assoc(seo_meta_robots_directives());
}

/**
 * Adds the shared basic metadata fields to a form container.
 */
function seo_meta_add_basic_elements(&$elements, $saved_tags, $options = array()) {
  $saved_tags = is_array($saved_tags) ? $saved_tags : array();
  $options += array(
    'page_title' => FALSE,
    'use_keywords' => TRUE,
    'keywords_states' => FALSE,
    'canonical' => FALSE,
    'default' => FALSE,
    'wrap_more' => FALSE,
  );

  if ($options['page_title']) {
    $elements['page_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Page title'),
      '#default_value' => isset($saved_tags['page_title']) ? $saved_tags['page_title'] : '',
      '#description' => t('Description of this page for the &lt;title&gt; tag. If empty, the normal page title is used.'),
    );
  }

  if ($options['use_keywords']) {
    $elements['meta_keywords'] = array(
      '#type' => 'textfield',
      '#title' => $options['default'] ? t('Default meta keywords') : t('Meta keywords'),
      '#default_value' => isset($saved_tags['meta_keywords']) ? $saved_tags['meta_keywords'] : '',
      '#size' => $options['default'] ? 70 : 120,
      '#maxlength' => 255,
      '#description' => t('Comma separated list of keywords (no more than 10 keyword phrases).'),
    );
    if ($options['keywords_states']) {
      $elements['meta_keywords']['#states'] = array(
        'invisible' => array(
          ':input[name="use_keywords"]' => array('checked' => FALSE),
        ),
      );
    }
  }

  $elements['meta_description'] = array(
    '#type' => 'textarea',
    '#title' => $options['default'] ? t('Default meta description') : t('Meta description'),
    '#default_value' => isset($saved_tags['meta_description']) ? $saved_tags['meta_description'] : '',
    '#rows' => 3,
    '#description' => t('One or two sentences describing this page (160-240 characters maximum, without HTML markup).'),
  );

  if ($options['wrap_more']) {
    $elements['more'] = array(
      '#type' => 'fieldset',
      '#title' => t('Robots and Canonical URL'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $more =& $elements['more'];
  }
  else {
    $more =& $elements;
  }

  $more['meta_robots'] = array(
    '#type' => 'checkboxes',
    '#title' => $options['default'] ? t('Default meta robots') : t('Meta robots'),
    '#options' => seo_meta_robots_options(),
    '#default_value' => isset($saved_tags['meta_robots']) ? seo_meta_robots_decode($saved_tags['meta_robots']) : array(),
    '#description' => t('If no page or default choice is set, the module outputs "index,follow".'),
    '#attributes' => array('class' => array('seo-meta')),
    '#element_validate' => array('seo_meta_robots_element_validate'),
  );

  if ($options['canonical']) {
    $more['rel_canonical'] = array(
      '#type' => 'textfield',
      '#title' => t('Canonical URL'),
      '#default_value' => isset($saved_tags['rel_canonical']) ? $saved_tags['rel_canonical'] : '',
      '#description' => t('Use an internal path or a complete HTTP/HTTPS URL. If empty, the current page URL is generated automatically.'),
      '#element_validate' => array('seo_meta_rel_canonical_validate'),
    );
  }
}

/**
 * Adds the shared Open Graph fields to a form container.
 */
function seo_meta_add_open_graph_elements(&$elements, $saved_tags, $config) {
  $saved_tags = is_array($saved_tags) ? $saved_tags : array();

  $elements['meta_og_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Open Graph title'),
    '#default_value' => isset($saved_tags['meta_og_title']) ? $saved_tags['meta_og_title'] : '',
    '#description' => t('The title used when this page is shared. If empty, the effective page title is used.'),
  );
  $elements['meta_og_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Open Graph description'),
    '#default_value' => isset($saved_tags['meta_og_description']) ? $saved_tags['meta_og_description'] : '',
    '#rows' => 3,
    '#description' => t('A brief description of the content (160-240 characters maximum, without HTML markup).'),
  );
  $elements['meta_og_type'] = array(
    '#title' => t('Open Graph type'),
    '#type' => 'select',
    '#options' => array(
      '' => t('Select type'),
      'website' => 'website',
      'article' => 'article',
      'product' => t('product (legacy)'),
      'profile' => 'profile',
    ),
    '#default_value' => isset($saved_tags['meta_og_type']) ? $saved_tags['meta_og_type'] : '',
    '#description' => t('The type of content. If other Open Graph values are configured and this is empty, "website" is used.'),
  );
  $elements['meta_og_image_fid'] = array(
    '#title' => t('Upload image that appears when someone shares the content.'),
    '#type' => 'managed_file',
    '#upload_location' => 'public://' . $config->get('meta_og_images_dir'),
    '#default_value' => isset($saved_tags['meta_og_image_fid']) ? $saved_tags['meta_og_image_fid'] : 0,
    '#upload_validators' => array(
      'file_validate_extensions' => array('gif png jpg jpeg'),
      'file_validate_size' => array(file_upload_max_size()),
      'file_validate_is_image' => array(),
    ),
  );
  $elements['meta_og_site_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Site name'),
    '#default_value' => isset($saved_tags['meta_og_site_name']) ? $saved_tags['meta_og_site_name'] : '',
    '#description' => t('The name of your website (such as IMDb, not imdb.com).'),
  );
}

/**
 * Adds the shared Twitter/X Card fields to a form container.
 */
function seo_meta_add_twitter_elements(&$elements, $saved_tags) {
  $saved_tags = is_array($saved_tags) ? $saved_tags : array();

  $elements['meta_tw_card'] = array(
    '#title' => t('Twitter/X Card type'),
    '#type' => 'select',
    '#options' => array(
      '' => t('Select card type'),
      'summary' => t('Summary Card'),
      'summary_large_image' => t('Summary Card with Large Image'),
    ),
    '#default_value' => isset($saved_tags['meta_tw_card']) ? $saved_tags['meta_tw_card'] : '',
    '#description' => t('The card type.'),
  );
  $elements['meta_tw_site'] = array(
    '#type' => 'textfield',
    '#title' => t('Site'),
    '#default_value' => isset($saved_tags['meta_tw_site']) ? $saved_tags['meta_tw_site'] : '',
    '#description' => t('@username of website.'),
  );
  $elements['meta_tw_creator'] = array(
    '#type' => 'textfield',
    '#title' => t('Creator'),
    '#default_value' => isset($saved_tags['meta_tw_creator']) ? $saved_tags['meta_tw_creator'] : '',
    '#description' => t('@username of content creator.'),
  );
  $elements['div'] = array(
    '#markup' => '<div class="description">' . t('The card title, description, and image are generated from the effective Open Graph/page values, so they do not need duplicate form fields.') . '</div>',
  );
}

/**
 * Validates mutually exclusive robots choices.
 */
function seo_meta_robots_element_validate($element, &$form_state, $form) {
  $selected = seo_meta_robots_decode($element['#value']);
  if (in_array('INDEX,FOLLOW', $selected, TRUE)
    && (in_array('NOINDEX', $selected, TRUE) || in_array('NOFOLLOW', $selected, TRUE))) {
    form_error($element, t('"INDEX, FOLLOW" cannot be selected together with "NOINDEX" or "NOFOLLOW".'));
  }
}

/**
 * Validates a page-specific canonical value.
 */
function seo_meta_rel_canonical_validate($element, &$form_state, $form) {
  $value = trim((string) $element['#value']);
  if ($value === '') {
    return;
  }

  if (strpos($value, '#') !== FALSE) {
    form_error($element, t('A canonical URL must not contain a fragment.'));
  }
  elseif (preg_match('@^https?://@i', $value)) {
    if (!valid_url($value, TRUE)) {
      form_error($element, t('Enter a valid HTTP or HTTPS canonical URL.'));
    }
  }
  elseif (strpos($value, '//') === 0 || preg_match('/^[a-z][a-z0-9+.-]*:/i', $value)) {
    form_error($element, t('Use an internal path or a complete HTTP/HTTPS URL.'));
  }
}

/**
 * Validates the optional canonical replacement hostname.
 */
function seo_meta_base_canonical_validate($element, &$form_state, $form) {
  $value = trim((string) $element['#value'], " \t\n\r\0\x0B/");
  if ($value !== '' && !preg_match('/^[a-z0-9.-]+(?::[0-9]{1,5})?$/i', $value)) {
    form_error($element, t('Enter only a hostname and optional port, without protocol or path.'));
  }
}

/**
 * Administration form for default seo meta tags.
 */
function seo_meta_settings_form() {
  global $language;
  $config         = config('seo_meta.settings');
  $core_config    = config('system.core');
  $saved_tags     = seo_meta_get_saved_tags(TYPE_DEFAULTS, $language->langcode);
  $excluded_types = $config->get('excluded_types');
  $excluded_types = is_array($excluded_types) ? $excluded_types : array();
  $form           = array();

  $form['langcode'] = array(
    '#type' => 'hidden',
    '#value' => $language->langcode,
  );
  $form['#attached']['css'] = array(
    backdrop_get_path('module', 'seo_meta') . '/css/seo_meta.admin.css',
  );
  $form['description'] = array(
    '#prefix' => '<div>',
    '#markup' => t('This module provides an interface for adding meta tags to pages.')
      . '<br />' . t('Values below are defaults. Non-empty page values override them; empty page fields inherit them.'),
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
    '#description' => t('Optional replacement hostname for automatically generated canonical URLs. Enter a hostname without protocol or path, for example: www.example.com'),
    '#element_validate' => array('seo_meta_base_canonical_validate'),
  );
  $form['basic']['use_keywords'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use keywords meta tag.'),
    '#default_value' => $config->get('use_keywords'),
    '#description' => t('If selected, the keywords field is enabled sitewide.'),
  );
  seo_meta_add_basic_elements($form['basic'], $saved_tags, array(
    'use_keywords' => TRUE,
    'keywords_states' => TRUE,
    'default' => TRUE,
  ));
  $form['basic']['meta_description']['#cols'] = 30;
  $form['basic']['fix_favicon'] = array(
    '#type' => 'checkbox',
    '#title' => t('Change shortcut icon link "rel" and "type" attributes'),
    '#default_value' => $config->get('fix_favicon'),
    '#description' => t('Change rel="shortcut icon" to rel="icon" and use type="image/x-icon".'),
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
    '#description' => t('Displayed between the page title and site name.'),
    '#element_validate' => array('seo_meta_site_name_divider_validate'),
  );

  $form['excludes'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['excludes']['excluded_types'] = array(
    '#title' => t('Excluded Content Types'),
    '#type' => 'checkboxes',
    '#options' => node_type_get_names(),
    '#default_value' => $excluded_types,
    '#description' => t('The selected content types will not contain metadata generated by this module.'),
    '#attributes' => array('class' => array('seo-meta')),
  );

  $form['open_graph'] = array(
    '#type' => 'fieldset',
    '#title' => t('Open Graph'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['open_graph']['use_og_url'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use "og:url" meta property.'),
    '#default_value' => $config->get('use_og_url'),
    '#description' => t('If selected, og:url is generated with exactly the same value as the page canonical URL.'),
  );
  seo_meta_add_open_graph_elements($form['open_graph'], $saved_tags, $config);

  $form['twitter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Twitter / X'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  seo_meta_add_twitter_elements($form['twitter'], $saved_tags);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  return $form;
}

/**
 * Implements hook form_validate().
 *
 */
function seo_meta_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  if (backdrop_strlen((string) $values['meta_description']) > 254) {
    form_set_error('meta_description', t('Text you are trying to enter in "Meta description" is too long.'));
  }
  if (backdrop_strlen((string) $values['meta_og_description']) > 254) {
    form_set_error('meta_og_description', t('Text you are trying to enter in "Open Graph description" is too long.'));
  }
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
  $values     = $form_state['values'];
  $config     = config('seo_meta.settings');
  $langcode   = $values['langcode'];
  $saved_tags = seo_meta_get_saved_tags(TYPE_DEFAULTS, $langcode);

  $config->set('base_canonical_url', trim((string) $values['base_canonical_url'], " \t\n\r\0\x0B/"));
  $config->set('use_keywords', (int) $values['use_keywords']);
  $config->set('use_og_url', (int) $values['use_og_url']);
  $config->set('fix_favicon', (int) $values['fix_favicon']);
  $config->set('append_site_name', (int) $values['append_site_name']);
  $config->set('site_name_divider', trim((string) $values['site_name_divider']));
  $config->set('excluded_types', is_array($values['excluded_types']) ? $values['excluded_types'] : array());
  $config->save();

  $data = array(
    'ce_type' => TYPE_DEFAULTS,
    'ce_id' => $langcode,
    'path_alias' => 'und',
    'langcode' => $langcode,
    'page_title' => 'und',
    'meta_keywords' => $values['use_keywords'] ? trim((string) $values['meta_keywords']) : '',
    'meta_description' => trim((string) $values['meta_description']),
    'meta_robots' => seo_meta_robots_encode($values['meta_robots']),
    'rel_canonical' => 'und',
    'meta_og_title' => trim((string) $values['meta_og_title']),
    'meta_og_description' => trim((string) $values['meta_og_description']),
    'meta_og_type' => (string) $values['meta_og_type'],
    'meta_og_image_fid' => (int) $values['meta_og_image_fid'],
    'meta_og_site_name' => trim((string) $values['meta_og_site_name']),
    'meta_tw_card' => (string) $values['meta_tw_card'],
    'meta_tw_site' => trim((string) $values['meta_tw_site']),
    'meta_tw_creator' => trim((string) $values['meta_tw_creator']),
    'uid' => $user->uid,
    'changed' => time(),
  );

  if ($saved_tags) {
    seo_meta_record_update(TYPE_DEFAULTS, $langcode, $data);
  }
  else {
    seo_meta_record_insert($data);
  }

  backdrop_set_message(t('The configuration options have been saved.'));
}

/**
 * Form wrapper for homepage
 */
function seo_meta_settings_homepage_form($form, &$form_state) {
  $sub_path = seo_meta_menu_sub_path();
  // We work with enabled languages only
  $site_langs = language_list(TRUE);

  if (count($site_langs) == 1) {
    $langcode = key($site_langs);
    backdrop_goto('admin/config/' . $sub_path . '/seo_meta/home/' . $langcode);
  }
  else {
    $form['#attached']['css'] = array(
      backdrop_get_path('module', 'seo_meta') . '/css/seo_meta.admin.css',
    );

    $form['description'] = array(
      '#markup' => t('This site have more than one content language enabled and you can define meta tags for front pages on each of them.'),
      '#weight' => -12,
    );

    foreach ($site_langs as $lang) {
      $langcode = $lang->langcode;
      $site_default_lang = $lang->default ? t('(site default language)') : '';
      $form_path = 'admin/config/' . $sub_path . '/seo_meta/home/' . $langcode;

      $form[$langcode . '_homepage'] =array(
        '#markup' => '<p>' . l(t('Meta tags for @lang @def front page.', array('@lang' => $lang->name, '@def' => $site_default_lang)), $form_path, array()) . '</p>',
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
  $sub_path    = seo_meta_menu_sub_path();
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
          '#markup' => l($view_name, 'admin/config/' . $sub_path . '/seo_meta/views/' . $view_name, array('attributes' => array('title' => t('Click to edit meta tags for this page.')))). '<br />',
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
  $sub_path = seo_meta_menu_sub_path();
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
          '#markup' => l($term_obj->name, 'admin/config/' . $sub_path . '/seo_meta/taxonomy/' . $term_obj->name, array('attributes' => array('title' => t('Click to edit meta tags for this page.')))). '<br />',
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
 */
function seo_meta_custom_item_tags_edit($form, &$form_state) {
  global $language;
  $config       = config('seo_meta.settings');
  $use_keywords = (bool) $config->get('use_keywords');
  $args         = arg();
  $page_type    = $args[4];

  if ($page_type == 'home') {
    $page_langcode = isset($args[5]) ? $args[5] : $language->langcode;
    $type       = TYPE_HOMEPAGE;
    $page_id    = $page_langcode;
    $site_langs = language_list(TRUE);
    if (!array_key_exists($page_langcode, $site_langs)) {
      form_set_error('', t('Language with code "@langcode" is not enabled for this site.', array('@langcode' => $page_langcode)));
      return $form;
    }
    $subtitle = t('For <em>@language</em> front page', array('@language' => language_name($page_langcode)));
  }
  else {
    $page_name = isset($args[5]) ? $args[5] : '';
    $type      = $page_type == 'views' ? TYPE_VIEW : TYPE_TAXONOMY;
    $page_id   = $page_name;
    $subtitle  = t('For @type page named <em>@name</em>', array('@type' => $page_type, '@name' => $page_name));
  }

  $saved_tags = seo_meta_get_saved_tags($type, $page_id);
  $form['page_info']['page_type'] = array('#type' => 'hidden', '#value' => $type);
  $form['page_info']['page_id'] = array('#type' => 'hidden', '#value' => $page_id);
  $form['page_info']['with_tags'] = array('#type' => 'hidden', '#value' => is_array($saved_tags));
  $form['page_info']['use_keywords'] = array('#type' => 'hidden', '#value' => $use_keywords);
  $form['#attached']['css'] = array(
    backdrop_get_path('module', 'seo_meta') . '/css/seo_meta.admin.css',
  );

  $form['seo_meta'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page title and meta tags'),
    '#access' => user_access('administer seo meta settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['seo_meta']['define'] = array(
    '#markup' => $subtitle . '<div class="description">' . t('Empty fields inherit corresponding defaults. An empty page title uses the normal page title.') . '</div>',
  );

  seo_meta_add_basic_elements($form['seo_meta'], $saved_tags, array(
    'page_title' => TRUE,
    'use_keywords' => $use_keywords,
    'canonical' => TRUE,
  ));

  $form['seo_meta']['open_graph'] = array(
    '#type' => 'fieldset',
    '#title' => t('Open Graph'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  seo_meta_add_open_graph_elements($form['seo_meta']['open_graph'], $saved_tags, $config);

  $form['seo_meta']['twitter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Twitter / X'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  seo_meta_add_twitter_elements($form['seo_meta']['twitter'], $saved_tags);

  $form['seo_meta']['actions'] = array('#type' => 'actions');
  $form['seo_meta']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Implements hook_validate().
 */
function seo_meta_custom_item_tags_edit_validate($form, &$form_state) {
  $values = $form_state['values'];

  if (backdrop_strlen((string) $values['meta_description']) > 254) {
    form_set_error('meta_description', t('Text you are trying to enter in "Meta description" is too long.'));
  }

  if (backdrop_strlen((string) $values['meta_og_description']) > 254) {
    form_set_error('meta_og_description', t('Text you are trying to enter in "Open Graph description" is too long.'));
  }
}

/**
 * Implements hook_submit().
 *
 */
function seo_meta_custom_item_tags_edit_submit($form, &$form_state) {
  global $user, $language;
  $values       = $form_state['values'];
  $page_type    = $values['page_type'];
  $page_id      = $values['page_id'];
  $use_keywords = !empty($values['use_keywords']);

  $data = array(
    'ce_type' => $page_type,
    'ce_id' => $page_id,
    'path_alias' => 'und',
    'langcode' => $language->langcode,
    'page_title' => trim((string) $values['page_title']),
    'meta_keywords' => $use_keywords ? trim((string) $values['meta_keywords']) : '',
    'meta_description' => trim((string) $values['meta_description']),
    'meta_robots' => seo_meta_robots_encode($values['meta_robots']),
    'rel_canonical' => trim((string) $values['rel_canonical']),
    'meta_og_title' => trim((string) $values['meta_og_title']),
    'meta_og_description' => trim((string) $values['meta_og_description']),
    'meta_og_type' => (string) $values['meta_og_type'],
    'meta_og_image_fid' => (int) $values['meta_og_image_fid'],
    'meta_og_site_name' => trim((string) $values['meta_og_site_name']),
    'meta_tw_card' => (string) $values['meta_tw_card'],
    'meta_tw_site' => trim((string) $values['meta_tw_site']),
    'meta_tw_creator' => trim((string) $values['meta_tw_creator']),
    'uid' => $user->uid,
    'changed' => time(),
  );

  if (!empty($values['with_tags'])) {
    seo_meta_record_update($page_type, $page_id, $data);
  }
  else {
    seo_meta_record_insert($data);
  }

  backdrop_set_message(t('Tags for page saved successfully.'));
}

/**
 * @return array of views pages with path defined
 */
function seo_meta_get_views_pages() {
  $result = array();
  $views = views_get_enabled_views();

  foreach ($views as $view) {
    if (empty($view->display)) {
      debug(t('Skipping broken view @view', array('@view' => $view->name)));
      continue;
    }

    foreach ($view->display as $display) {
      if ($display->display_plugin != 'page' || empty($display->display_options['path'])) {
        continue;
      }

      $page_path = $display->display_options['path'];
      if (strpos($page_path, 'admin/') === FALSE) {
        $result[][$view->name] = $page_path;
      }
    }
  }

  return $result;
}


