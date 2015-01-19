step_interval = 0.0625; %us
ramp_time = 5000; %us


adj_db =0:-0.5:-127;
adj_amp =db2mag(adj_db);

ramp_step_num_all = ramp_time/step_interval;

syms x;
y = cos(x / ramp_step_num_all * pi /2);
% y = (cos(x / ramp_step_num_all * pi /2) + adj_amp(end))*(1 - adj_amp(end));
% y = (cos(x / ramp_step_num_all * pi /2) + adj_amp(end));
f=finverse(y);



% adj_step_ind = subs(f,{x,pi},{adj_amp,3.14});
 adj_step_ind = double(subs(f,adj_amp));
 j=1;
 result_step_isi = [];
 result_adj_vol = [];
 temp_ind=0;
 j=1;
 for i = 1:length(adj_step_ind)
     
     if (adj_step_ind(i) - temp_ind)>240
         result_step_isi(j) = int32(adj_step_ind(i)) - temp_ind;
         temp_ind = int32(adj_step_ind(i));
         result_adj_vol(j) = i-1;
         j=j+1;
     end
 end
 
 %% test result
 
 
 test_time_ind = [];
 test_time_ind(1) = 0;
 for i = 1:length(result_step_isi)
     test_time_ind(i+1)=test_time_ind(i)+result_step_isi(i);
 end
 
 
 test_amp = [1,db2mag(-result_adj_vol/2)];

 plot(test_time_ind/test_time_ind(end)*pi/2,test_amp);
 
 %% test2
%  figure;
hold on;
 plot([0:0.01:pi/2],cos([0:0.01:pi/2]));
 
 
 