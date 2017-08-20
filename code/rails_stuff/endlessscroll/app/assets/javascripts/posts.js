$(function () {
  if ($(".pagination").length) {
    $(window).scroll(function () {
      var loadOffset = 100;
      var url = $(".pagination a[rel=next]").attr("href");
      var breakingPoint = $(document).height() - $(window).height() - loadOffset;

      if (url && $(window).scrollTop() >= breakingPoint) {
        $(".pagination").html("Loading...");
        return $.getScript(url);
      }
      else if (!url) {
        $(".pagination").html("No content");
      }
    });
  }
});
