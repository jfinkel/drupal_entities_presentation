(function ($) {

  Drupal.behaviors.donations = {
    attach: function (context, settings) {
      $('#edit-name', context).change(function () {
        var names = this.value.split(" ");
        if (names.length == 2)
        {
          var answer = names[1] + ',' + names[0]
        }
        else
        {
          var answer = this.value;
        }
        $('#edit-sort-name').val(answer);    
      });
    }
  };

})(jQuery);

