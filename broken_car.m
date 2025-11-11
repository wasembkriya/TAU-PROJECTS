classdef broken_car < car
    properties
        faults, prices ,status
    end
    methods
        function obj= broken_car(c,faults)
            obj@car(c.number,c.owner);
            obj.faults=faults;
            [fault_list, price_list] = obj.load_pricelist();
            
            % Match the faults with their prices
            obj.prices = zeros(size(faults));
            for i = 1:length(faults)
                index = find(strcmp(fault_list, faults{i}));
                if ~isempty(index)
                    obj.prices(i) = price_list(index);
                else
                    obj.prices(i) = NaN; 
                end
            end
            obj.status='pending';
        end
        function fcar=fix(obj)
            if isa(obj,'fixed_car')
                error('fwafasfw');
            else
                obj.status='handled';
                fcar = fixed_car(obj);
            end
        end
    end
end
