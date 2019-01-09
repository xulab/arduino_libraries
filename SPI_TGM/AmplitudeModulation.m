step_interval = 0.0625; %us
half_periode_time = 20000; %us
minISI = 0.01; 
minAmpInterval = 0.001;

% AdjVol_temp = [];
% i = 1;
% AdjVol_temp(1) = 0;
% for j = 0:-1:-192 %maximum volume value is 192.
%     lastAmp = db2mag(AdjVol_temp(i));
%     tempAmp = db2mag(j);
%     AmpIntervalTemp = lastAmp - tempAmp;
%     ISIIntervalTemp = acos(tempAmp*2-1)/pi - acos(lastAmp*2-1)/pi;
%     
%     if ( AmpIntervalTemp < minAmpInterval )
%         continue
%     end
%     if ( ISIIntervalTemp < minISI )
%         continue
%     end
%     i = i + 1;
%     AdjVol_temp(i) = j;
% end
% AdjVol_temp(i+1)=-192;
% resulVol = abs(AdjVol_temp(2:end));
% adj_amp =10.^(AdjVol_temp/2/20);
% adj_temp = acos(adj_amp*2-1)/pi;
% figure;
% plot(adj_temp, adj_amp, ".");
% 
% hold on
% x = [0:0.01:1];
% plot(x,(cos(x*pi)+1)/2);
% %% calculate interval
% resultISI = [];
% 
% for i = 1:(length(adj_temp)-1)
%     resultISI(i) = adj_temp(i+1) - adj_temp(i) ;
% end
% figure
% hist(resultISI, 100);
% sum(resultISI)
% %%
% AMP_interval = [];
% for i = 1:(length(adj_amp)-1)
%     AMP_interval(i) = adj_amp(i) - adj_amp(i+1) ;
% end
% figure
% hist(AMP_interval, 100);
% figure
% plot(resultISI, AMP_interval, ".");
% 


%%
AdjVol_temp = 0:-1:-192;
adj_amp =10.^(AdjVol_temp/2/20);
rawTimeStamp = acos(adj_amp*2-1)/pi;
% rawTimeStamp = acos(db2mag((0:-1:-192)/2)*2-1)/pi;
rawISI = [];
for i = 1:192
   rawISI(i) = rawTimeStamp(i + 1) -  rawTimeStamp(i); 
end
figure;
plot(rawTimeStamp,adj_amp,".");
hold on
x = [0:0.01:1];
plot(x,(cos(x*pi)+1)/2);
