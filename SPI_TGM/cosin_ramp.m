a=0:-0.5:-1000;
b=db2mag(a);
c=acos(b)/pi*2;
d = [];
for i = 1:2000
    d(i) = c(i+1) - c(i);
end
d = d*1000;
d = round(d/0.0625);
i = 1;
step = [];
step_isi = [];
temp1 = 0;
temp_step = 1;
for k = 1:length(d)
    temp1 = temp1 + d(k);
    if temp1 > 240 
        step_isi(i) = temp1;
        step(i) = k;
        temp_step = 1;
        temp1 =0;
        i = i+1;
    else 
        temp_step = temp_step+1;
    end
end
