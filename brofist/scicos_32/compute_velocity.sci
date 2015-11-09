function [v] = compute_velocity(p)

S = max(size(p));

start_pos = p(1,2);

p_filtered = [[p(1,1),start_pos]];
for i=1:S,
    if p(i,2) ~= start_pos then
        p_filtered = [p_filtered; [p(i,1), p(i,2)]];
        start_pos = p(i,2);
    end,
end

v = []

S = max(size(p_filtered));

for i=2:S,
    nv = (p_filtered(i,2)-p_filtered(i-1,2))/(p_filtered(i,1)-p_filtered(i-1,1));
    v = [v;[p_filtered(i,1) nv]];
end

plot(v (:,1),v(:,2));

endfunction
