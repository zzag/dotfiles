from django.conf.urls import url
from tmp import views


urlpatterns = [
    url(r'^$', views.index),
]
