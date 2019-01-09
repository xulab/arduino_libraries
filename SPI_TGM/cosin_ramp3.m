%% rawInterval
AdjVol_temp = -192 : 1 : 0;
adj_amp =10.^(AdjVol_temp/2/20);
rawTimeStamp = acos(1 - adj_amp*2)/pi;
% rawTimeStamp = acos(db2mag((0:-1:-192)/2)*2-1)/pi;
rawInterval = [];
for i = 1:192
   rawInterval(i) = rawTimeStamp(i + 1) -  rawTimeStamp(i); 
end
rawInterval(1) = rawInterval(1) + rawTimeStamp(1);
figure;
plot(rawTimeStamp,adj_amp,".");
hold on
x = [0:0.01:1];
plot(x,(-cos(x*pi)+1)/2);
 
%% calculate Interval & adjVal
%% const
ramp_time = 5000; %us
minInterval = 10; %10us


minInterval_z = minInterval/ramp_time;
interval_temp = 0;
result_interval = [];
result_vol = [];
j = 1;
for i = 1:192
   interval_temp = interval_temp + rawInterval(i);
   if(interval_temp>= minInterval_z)
       result_interval(j) =  interval_temp;
       result_vol(j) = 192 - i;
       j = j + 1; 
       interval_temp = 0;
   end
end

if(0 ~= result_vol(end)) 
    result_interval(j) =  minInterval_z;
    result_vol(j) = 0;
end
result_interval = result_interval.*ramp_time;