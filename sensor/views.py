from django.shortcuts import render #render: truy xuat du lieu
from django.utils import timezone
from .models import Sensor

# Create your views here.

def sensor(request):
	sensor = Sensor.objects.order_by('-id')[:5] # lay 5 gia tr dau tien
	time = Sensor.objects.latest('Date_and_Time')


	return render(request, 'sensor/sensor.html', {'sensor':sensor,'time':time})
# Create your views here.
