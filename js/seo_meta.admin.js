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

  /**
   * Try automatically fill in a page title.
   * Same as in core menu module
   */
  Backdrop.behaviors.seo_metaAutomaticTitle = {
    attach: function (context) {
      $(context).find('fieldset#edit-seo-meta').each(function () {
        var $checkbox = $('#edit-seo-meta-enabled', this);
        var $page_title = $('#edit-seo-meta-page-title', context);
        var $title = $(this).closest('form').find('.form-item-title input');

        if (!($checkbox.length && $page_title.length && $title.length)) {
          return;
        }
        // If there is a page title already, mark it as overridden. The user expects
        // that toggling the checkbox twice will take over the node's title.
        if ($checkbox.is(':checked') && $page_title.val().length) {
          $page_title.data('seo_metaAutomaticTitleOveridden', true);
        }
        // Whenever the value is changed manually, disable this behavior.
        $page_title.keyup(function () {
          $page_title.data('seo_metaAutomaticTitleOveridden', true);
        });
        // Global trigger on checkbox (do not fill-in a value when disabled).
        $checkbox.change(function () {
          if ($checkbox.is(':checked')) {
            if (!$page_title.data('seo_metaAutomaticTitleOveridden')) {
              $page_title.val($title.val());
            }
          }
          else {
            $page_title.val('');
            $page_title.removeData('seo_metaAutomaticTitleOveridden');
          }
          $checkbox.closest('fieldset.vertical-tabs-pane').trigger('summaryUpdated');
          $checkbox.trigger('formUpdated');
        });
        // Take over any title change.
        $title.keyup(function () {
          if (!$page_title.data('seo_metaAutomaticTitleOveridden') && $checkbox.is(':checked')) {
            $page_title.val($title.val());
            $page_title.val($title.val()).trigger('formUpdated');
          }
        });
      });
    }
  };
  
})(jQuery);

