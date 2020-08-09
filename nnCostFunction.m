function [ J grad ] = nnCostFunction( nn_params, s, X, y, lambda )
% COSTFUNCTION Cost function for neural networks performing classification.
% ===========================================================================
%   [ J grad ] = nnCostFunction( nn_params, s, X, y, lambda ) Implements a 
%   nueral network cost function.
%
%	nn_params : Parameters for the nerual network as a single column vector.
%	s         : A vector containing the number of units per layer.
%	X         : Dataset 
%	y         : Classifications
%	lambda    : Regularization parameter.
%
%	J         : Cost function value
%       grad      : Cost function gradient.
% ===========================================================================

m = size( X, 1 );       % number of training examples
L = length( s );        % number of layers
K = s( end )            % number of output units
J = 0;                  % cost
grad = [];              % gradient


% reshape parameters into weight matrices
theta = {}; 
last = 0;
for l = 1:( L - 1)
    rows = s( l + 1 );
    cols = s( l ) + 1;
    first = last + 1;
    last = first + rows * cols - 1;
    theta{ l } = reshape( nn_params( first:last ), rows, cols );
end


% map classifications into binary vectors
y_bin = zeros( m, K ); 
for i = 1:m
   y_bin( i , : ) = ( [ 1:K ] == y( i ) );
end

% --------------------------------------------------------------------------
% Feedforward : Propagate forward to compute activation unit values
% --------------------------------------------------------------------------

a = { X };  % activation units
z = {};     % sigmoid parameters

% Compute activation values from previous layers
for l = 1:( L - 1 )
    % add bias unit
    a{ l } = [ ones( m, 1 ), a{ l } ]; 
    z{ l + 1 } = a{ l } * theta{ l }';
    a{ l + 1 } = sigmoid( z{ l + 1 } );
end


% compute unregularized cost
for i = 1:m
    y_i = y_bin( i , : )'; 
    h_i = a{ L }( i , : )';   % output values h( x(i) )
    J = J + ( 1 / m ) * ...
        sum( y_i .* log( h_i ) + ( 1 - y_i ) .* log( 1 - h_i ) );
end


% Add regularization term to cost
for l = 1:( L - 1 )
    t = theta{ l }( : , 2:end )( : ) % unrolled params; ignoring bias
    J = J + ( 1 / 2 ) * ( lambda / m ) * sum( t .^ 2 );
end


% ---------------------------------------------------------------------------
% Backprop : Propagate backwards to compute gradient
% ---------------------------------------------------------------------------

delta = {};    % ~ unit "error" vectors
Delta = {};    % "error" accumulator matrices
for l = 1:( L - 1 )
    Delta{ l } = zeros( size( theta{ l } ) );
    delta{ l + 1 } = zeros( s( l + 1 ), 1 );
end


% loop through each training example
for i = 1:m

    % initial delta - difference between predicted and actual value
    delta{ L } = a{ L }( i, : )' - y_bin( i, : )';

    % compute delta(l - 1) from delta(l)
    for l = ( L - 1 ):-1:2
        % add bias for sigmoid function
        z_l = [ 1; z{ l }( i, : )' ]; 
        delta{ l } = ( ...
            theta{ l }' * delta{ l + 1 } ) .* sigmoidGradient( z_l ) );
        % remove bias
        delta{ l } = delta{ l }( 2:end );  
    end

    % Update Delta
    for l = 1:( L - 1 )
        Delta{ l } = Delta{ l } + ( delta{ l + 1 } * a{ l }( i, : ) );
    end

end

% Compute partial derivatives.
D = {};
for l = 1:( L - 1 )
    reg_term = ( lambda / m ) * theta{ l };              % regularization term
    reg_term( :, 1 ) = zeros( size( reg_terms, 1 ), 1 ); % ignore bias
    D{ l } = ( 1 / m ) * Delta{ l } + reg_term;
end


% Combine partial derivatives into column vector for the gradient.
for l = 1:( L - 1)
    grad = [ grad; D{ l }( : ) ];
end

end
