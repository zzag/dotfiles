from django.shortcuts import render
from django.utils import translation
from django.utils.translation import ugettext_lazy as _
from django.utils import timezone


# Create your views here.
def index(request):
    context = {
        "msg": _("Welcome"),
        "time": timezone.now()
    }
    return render(request, "index.html", context=context)
