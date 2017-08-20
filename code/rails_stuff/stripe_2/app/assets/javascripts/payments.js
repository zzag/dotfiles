function stripeResponseHandler(status, response) {
  var $form = $('#new_payment');

  if (response.error) {
    alert(response.error.message);
    $form.find('button').prop('disabled', false);
  }
  else {
    var token = response.id;
    $form.append($("<input type=\"hidden\" name=\"token\" />").val(token));
    $form.find('[data-stripe=number]').remove();
    $form.find('[data-stripe=cvv]').remove();
    $form.find('[data-stripe=exp-year]').remove();
    $form.find('[data-stripe=exp-month]').remove();

    $form.get(0).submit();
  }
}

$(function () {
  $('#new_payment').submit(function (event) {
    var $form = $(this);
    $form.find('button').prop('disabled', true);
    Stripe.card.createToken($form, stripeResponseHandler);
    return false;
  });
});
