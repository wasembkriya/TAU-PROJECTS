classdef fixed_car < car
    properties
        bill
    end
    methods
        function obj=fixed_car(br_car)
            obj@car(br_car.number,br_car.owner)
            obj.bill=sum(br_car.prices);
        end
    end
end


