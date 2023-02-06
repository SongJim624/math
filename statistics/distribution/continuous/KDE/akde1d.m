function [pdf,grid]=akde1d(X,grid,gam)
%% fast adaptive kernel density estimation in one-dimension;
%  provides optimal accuracy/speed tradeoff, controlled with parameter "gam";
% INPUTS:   X  - data as a 'n' by '1' vector;
%
%         grid - (optional) mesh over which density is to be computed;
%                default mesh uses 2^12 points over range of data;
%
%          gam - (optional) cost/accuracy tradeoff parameter, where gam<n;
%                default value is gam=ceil(n^(1/3))+20; larger values
%                may result in better accuracy, but always reduce speed;
%                to speedup the code, reduce the value of "gam"; 
%
% OUTPUT: pdf - the value of the estimated density at 'grid'
%
%%  EXAMPLE:
%   data=[exp(randn(10^3,1))]; % log-normal sample
%   [pdf,grid]=akde1d(data); plot(grid,pdf)
%
% Note: If you need a very fast estimator use my "kde.m" function.
% This routine is more adaptive at the expense of speed. Use "gam"
% to control a speed/accuracy tradeoff. 
%
%%  Reference:
%  Kernel density estimation via diffusion
%  Z. I. Botev, J. F. Grotowski, and D. P. Kroese (2010)
%  Annals of Statistics, Volume 38, Number 5, pages 2916-2957.
X = load('D:\Visual Studio Projects\Unit Test for the Matrix class\Unit Test for the Matrix class\result.txt');
Xc = X;
n = length(X);
% begin scaling preprocessing
scaling = 0.83 / (max(X) - min(X));
MIN = 1.1 * min(X) - 0.1 * max(X);
X = (X - MIN) * scaling;
gam = ceil(n^(0.3))+20;
% end preprocessing
% algorithm initialization
del= 0.2 * n^(-0.3);
mu=X(1 : gam);
w=rand(1, gam);
sum = 0;
for c = 1 : gam
    sum = sum + w(c);
end

for c = 1 : gam
    w(c) = w(c) / sum;
end
Sig=del * del * rand(1, gam);
ent=-Inf;
for iter= 1 : 1 % begin algorithm
    Eold=ent;
    [w,mu,Sig,del,ent]=regEM_MY(w,mu,Sig,del,X); % update parameters
    err=abs((ent-Eold)/ent); % stopping condition
    fprintf('Iter.    Tol.      Bandwidth \n');
    fprintf('%4i    %8.2e   %8.2e\n',iter,err,del);
    fprintf('----------------------------\n');
    if (err<1e-6) break, end
end

DATA = load('D:\Visual Studio Projects\Unit Test for the Matrix class\Unit Test for the Matrix class\parameters.txt');
mu = DATA(:, 1);
Sig = DATA(:, 2);
w = DATA(:, 3);

% now output density values at grid
pdf = probfun(X,w,mu,Sig) * prod(scaling); % evaluate density
plot(Xc, pdf, '.');
del=del * scaling; % adjust bandwidth for scaling
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [w, mu, Sig, del, ent] = regEM_MY(w, mu, Sig, del, X)
% here we assume d = 1
% and w, mu, sig are all the row vectors
gam = length(mu);
n = length(X);

log_lh = zeros(n,gam); 
log_sig = zeros(n, gam);

maxll = zeros(1, n);
maxlsig = zeros(1, n);

density = zeros(1, n);
psigd = zeros(1, n);

for r = 1 : n
    for c = 1 : gam
        log_lh(r, c) = X(r) - mu(c);
        log_lh(r, c) = log_lh(r, c).^2 / Sig(c);
        log_sig(r, c) = log_lh(r, c) / Sig(c) + eps;
        temp = 1.8379 + log(Sig(c) / (w(c) * w(c))) + del * del / Sig(c);
        log_lh(r, c)= -0.5 * (log_lh(r, c) + temp);
        log_sig(r, c) = log_lh(r, c) + log(log_sig(r, c));
    end
end

for r = 1 : n
    maxll(r) = -Inf;
    maxlsig(r) = -Inf;
    for c = 1 : gam    
        maxll(r) = max (log_lh(r, c),maxll(r)); 
        maxlsig(r) = max (log_sig(r, c), maxlsig(r));
    end
end

for r = 1 : n
    for c = 1 : gam
        log_lh(r, c) = exp(log_lh(r, c) - maxll(r));
        log_sig(r, c) = exp(log_sig(r, c) - maxlsig(r));
    end
end

for r = 1 : n
    for c = 1 : gam
        density(r) = density(r) + log_lh(r, c);
        psigd(r) = psigd(r) + log_sig(r, c);
    end
end

for r = 1 : n
    for c = 1 : gam
        log_lh(r, c) = log_lh(r, c) / density(r);
    end
end

for c = 1 : gam
    w(c) = 0;
    for r = 1 : n
        w(c) = w(c) + log_lh(r, c);
    end
end

ent = 0;

for r = 1 : n
    density(r) = log(density(r)) + maxll(r);
    psigd(r) = log(psigd(r)) + maxlsig(r);
    ent = ent + density(r);
end

for c = 1 : gam
    if(w(c) <= 0)
        continue;
    end
   
    mu(c) = 0;
    
    for r = 1 : n
        mu(c) = mu(c) + log_lh(r, c) * X(r);
    end
    mu(c) = mu(c) / w(c);
    
    Sig(c) = 0;
    for r = 1 : n
        Sig(c) = Sig(c) + log_lh(r, c) * (X(r) - mu(c))^ 2;
    end
    
    Sig(c) = Sig(c) / w(c) + del * del;    
end

sum = 0;
for c = 1 : gam
    sum = sum + w(c);
end

for c = 1 : gam
    w(c) = w(c) / sum;
end
curv = 0;
for r = 1 : n
    curv = curv + exp(psigd(r) - density(r));
end
curv = curv / n;
del = (14.17963 * curv * n)^(-0.3);

end

function [w,mu,Sig,del,ent] = regEM(w,mu,Sig,del,X)
[gam,d]=size(mu);[n,d]=size(X);
log_lh=zeros(n,gam); log_sig=log_lh;
for i=1:gam
    s=Sig(i,:);
    Xcentered = bsxfun(@minus, X, mu(i,:));
    xRinv = bsxfun(@rdivide, Xcentered.^2, s);
    xSig = sum(bsxfun(@rdivide, xRinv, s),2)+eps;
    log_lh(:,i)=-.5*sum(xRinv, 2)-.5*sum(log(s))...
        +log(w(i))-d*log(2*pi)/2-.5*del^2*sum(1./s);
    log_sig(:,i)=log_lh(:,i)+log(xSig);
end
maxll = max (log_lh,[],2); maxlsig = max (log_sig,[],2);
p= exp(bsxfun(@minus, log_lh, maxll));
psig=exp(bsxfun(@minus, log_sig, maxlsig));
density = sum(p,2); psigd=sum(psig,2);
logpdf=log(density)+maxll; logpsigd=log(psigd)+maxlsig;
p = bsxfun(@rdivide, p, density);% normalize classification prob.
ent=sum(logpdf); w=sum(p,1);
for i=find(w>0)
    mu(i,:)=p(:,i)'*X/w(i);  %compute mu's
    Xcentered = bsxfun(@minus, X,mu(i,:));
    Sig(i,:)=p(:,i)'*(Xcentered.^2)/w(i)+del^2; % compute sigmas
end
w=w/sum(w);
curv=mean(exp(logpsigd-logpdf));
del=1/(4*n*(4*pi)^(d/2)*curv)^(1/(d+2));
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Gaussian Kernel
function out=probfun(x,w,mu,Sig)
[gam,d]=size(mu);
out=0;
for k=1:gam
    S=Sig(k);
    xx=bsxfun(@minus, x,mu(k));
    xx=bsxfun(@rdivide,xx.^2,S);
    out=out+exp(-.5*sum(xx,2)+log(w(k))-.5*sum(log(S))-d*log(2*pi)/2);
end
end