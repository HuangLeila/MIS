%integral function of demand uniform distribution
function y=integratedUniform(x,q,lower_bound,upper_bound)
f1=(x-q).*(1/(upper_bound-lower_bound));
y=integral(@(x)f1,q,upper_bound);
end

%integral function of demand normal distribution
function y=integratedNormal(x,q,expected_value,variance)
f1=(x-q).*((1/sqrt(pi*2))*exp(-(x-expected_value).^2/(2*variance)));
y=integral(@(x)f1,q,inf);
end

%total cost under order quantity q
function y=grossCost(q,p,s,c,v,x)
if q<x
    y=(s+p-c)*q-p*x;
else
    y=(s-v)*x-(c-v)*q;
end
end

%compute excepted demand surplus regarding to current quantity
function y=expectedDemandSurplus(q,exp)
if exp<q
    y=0;
else
    y=exp-q;
end
end
    


