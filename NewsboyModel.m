%penalty, sale price, cost, residual value, total budget
p=10;
s=8;
c=6;
v=4;
w=500;
w_l=1000;
% assume the market demand obey uniform distribution
l_b=75;
u_b=125;
syms exp variance
exp=(l_b+u_b)/2;
variance=(u_b-l_b)^2/2;
%assume the market demand obey normal distribution
exp_n=100;
variance_n=1200;

%P1:budget constraint with uniform distribution
cvx_begin
cvx_precision low
variable q nonnegative
maximize((s-v)*exp-(c-v)*q-(s+p-v)*integratedUniform(x,q,l_b,u_b));
subject to
    c*q-w<=0;
cvx_end

%generate demand by uniform distribution
xd=unifrnd(l_b,u_b,1,1);
xd

%define arrays for quantities and gross cost for model P1
P1q=[];
P1gc=[];

%verification and validation
for i=-50:+10:50
    P1q=[P1q,q+i];
    P1gc=[P1gc,grossCost(q+i,p,s,c,v,xd)];
end
plot(P1q,P1gc);
grid on;
xlabel('order quantities');
ylabel('gross cost under order quantities');
hold on;

%P1:budget constraint with normal distribution
cvx_begin
cvx_precision low
variable q1 nonnegative
maximize((s-v)*exp-(c-v)*q1-(s+p-v)*integratedNormal(x,q1,exp_n,variance_n));
subject to
    c*q1-w<=0;
cvx_end

%generate demand by uniform distribution
xd1=normrnd(exp_n,variance_n,1,1);
xd1

%verification and validation
for i=-50:+10:50
    P1q1=[P1q,q1+i];
    P1gc1=[P1gc,grossCost(q1+i,p,s,c,v,xd1)];
end
plot(P1q1,P1gc1);
grid on;
%xlabel('order quantities');
%ylabel('gross cost under order quantities');
hold on;

%P2:budget includes penalty constraint with uniform distribution
cvx_begin
cvx_precision low
variable q2 nonnegative
maximize((s-v)*exp-(c-v)*q2-(s+p-v)*integratedUniform(x,q2,l_b,u_b));
subject to
    c*q2-p*expectedDemandSurplus(exp,q2)-w<=0;
cvx_end

%generate demand by uniform distribution
xd2=unifrnd(l_b,u_b,1,1);
xd2

%define arrays for quantities and gross cost for model P1
P2q=[];
P2gc=[];

%verification and validation
for i=-50:+10:50
    P2q=[P2q,q2+i];
    P2gc=[P2gc,grossCost(q2+i,p,s,c,v,xd2)];
end
plot(P2q,P2gc);
grid on;
%xlabel('order quantities');
%ylabel('gross cost under order quantities');
hold on;

%P2:budget include penalty constraint with normal distribution
cvx_begin
cvx_precision low
variable q3 nonnegative
maximize((s-v)*exp-(c-v)*q3-(s+p-v)*integratedNormal(x,q3,exp_n,variance_n));
subject to
    c*q3-p*expectedDemandSurplus(exp,q3)-w<=0;
cvx_end

%generate demand by uniform distribution
xd3=normrnd(exp_n,variance_n,1,1);
xd3

%define arrays for quantities and gross cost for model P1
P2q2=[];
P2gc2=[];

%verification and validation
for i=-50:+10:50
    P2q1=[P2q1,q3+i];
    P2gc1=[P2gc1,grossCost(q3+i,p,s,c,v,xd3)];
end
plot(P2q1,P2gc1);
grid on;
%xlabel('order quantities');
%ylabel('gross cost under order quantities');
hold on;

%P3:budget includes holding cost constraint with uniform distribution
cvx_begin
cvx_precision low
variable q4 nonnegative
maximize((s-v)*exp-(c-v)*q4-(s+p-v)*integratedUniform(x,q4,l_b,u_b));
subject to
    (c+v)*q4-v*exp-w<=0;
cvx_end

%generate demand by uniform distribution
xd4=unifrnd(l_b,u_b,1,1);
xd4

%define arrays for quantities and gross cost for model P1
P3q=[];
P3gc=[];

%verification and validation
for i=-50:+10:50
    P3q=[P3q,q4+i];
    P3gc=[P3gc,grossCost(q4+i,p,s,c,v,xd4)];
end
plot(P3q,P3gc);
grid on;
%xlabel('order quantities');
%ylabel('gross cost under order quantities');
hold on;

%P3:budget include holding cost constraint with normal distribution
cvx_begin
cvx_precision low
variable q5 nonnegative
maximize((s-v)*exp-(c-v)*q5-(s+p-v)*integratedNormal(x,q5,exp_n,variance_n));
subject to
    (c+v)*q5-v*exp_n-w<=0;
cvx_end

%generate demand by uniform distribution
xd5=normrnd(exp_n,variance_n,1,1);
xd5

%define arrays for quantities and gross cost for model P1
P3q2=[];
P3gc2=[];

%verification and validation
for i=-50:+10:50
    P3q1=[P3q1,q5+i];
    P3gc1=[P3gc1,grossCost(q5+i,p,s,c,v,xd5)];
end
plot(P3q1,P3gc1);
grid on;
%xlabel('order quantities');
%ylabel('gross cost under order quantities');
%hold on;

