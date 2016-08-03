(function ($) {
/**
 * Update the summary for the module's vertical tab.
 */
Backdrop.behaviors.seo_metaFieldsetSummaries = {
  attach: function (context) {
    $('fieldset#edit-seo-meta', context).backdropSetSummary(function (context) {
      if ($('#edit-seo-meta-enabled', context).is(':checked')) {
        return Backdrop.t('Tags defined');
      }
      else {
        return Backdrop.t('No tags defined');
      }
    });
  }
};

})(jQuery);

