function [x,y,typ] = SPAM_Sensor(job,arg1,arg2)
    x=[];y=[];typ=[];
    sensor = '';
    port_no = '';
    select job
        case 'plot' then
            graphics=arg1.graphics;
            ierr=execstr('(evstr(graphics.exprs(1))==1)','errcatch')
            if ierr<>0 then
                sensor = 'Custom Sensor';                
            end
            printf("INIT Sensor: %s, port: %s\n", graphics.exprs(1), graphics.exprs(2));
            select evstr(graphics.exprs(1))
                case 1 then
                    sensor = 'Tacho Count'
                case 2 then
                    sensor = 'AVG Speed'
                case 3 then
                    sensor = 'Light Sensor'
                case 4 then
                    sensor = 'Touch Sensor'
                case 5 then
                    sensor = 'Sound Sensor'
                case 6 then
                    sensor = 'Ultrasonic Sensor'
                else
                    sensor = '?'
            end
            select evstr(graphics.exprs(2))
                case 1 then
                    port_no = '1'
                case 2 then
                    port_no = '2'
                case 3 then
                    port_no = '3'
                case 4 then
                    port_no = '4'
                case 5 then
                    port_no = 'A'
                case 6 then
                    port_no = 'B'
                case 7 then
                    port_no = 'C'
                else
                    port_no = '?'
            end
            standard_draw(arg1)
        case 'getinputs' then
            [x,y,typ]=standard_inputs(arg1)
        case 'getoutputs' then
            [x,y,typ]=standard_outputs(arg1)
        case 'getorigin' then
            [x,y]=standard_origin(arg1)
        case 'set' then
            x = arg1;
            graphics = arg1.graphics;
            exprs = graphics.exprs;
            model = arg1.model;
            while %t do
                [ok,sensor_typ,port,exprs]=getvalue('Set Sensors Parameters',..
                    ['Sensor Type (1~6)';'Port (1~4 for Sensors Port, 5~7 for A->C)'],..
                    list('vec',1,'vec',1),exprs)
                if ~ok then
                    break
                end
                sensor_typ = int(sensor_typ);
                port = int(port);
                printf("Sensor: %d, port: %d\n", sensor_typ, port);
                if (((sensor_typ < 1) | (sensor_typ > 6)) | ((port < 1) | (port > 7)) ) then
                    printf("Numero sensore NON ok\n");
                    message('Sensor Type can only be a number between 1 and 6');
                elseif (((sensor_typ >= 3) & (sensor_typ <= 6)) & ((port < 1) | (port > 4))) then 
                    printf("Sensore settato sulla porta sbagliata\n");
                    message ('Port number should be 1 to 4 for sensors');
                elseif (((port < 5) | (port > 7)) & ((sensor_typ == 1) | (sensor_typ == 2))) then
                    printf("Sensore di motore settato sulle porte 1~4!\n");
                    message ('Port number should be 5 to 7 for motor ports A to C');
                else
                    printf("Post if: %d, port: %d\n", sensor_typ, port);
                    graphics.exprs = exprs; 
                    model.dstate = 0;
                    model.ipar = [sensor_typ; port];
                    x.graphics = graphics;
                    x.model = model;
                    break
                end
            end
        case 'define' then
            sensor_typ = 1;
            port = 5;
            model=scicos_model()
            model.sim=list('bro_sensor_enc',4)
            model.out = [3];
            model.out2 = [1];
            model.outtyp = [1];
            model.in=[]
            model.evtin=[]
            model.rpar=[]
            model.ipar = [sensor_typ; port];
            model.dstate=[1];
            model.blocktype='c'
            model.dep_ut=[%t %f]
            exprs = [string(sensor_typ);string(port)];
            gr_i=['xstringb(orig(1),orig(2),[sensor+'' on Port ''+port_no],sz(1),sz(2),''fill'');']
            x=standard_define([3 2],model,exprs,gr_i)
        end
endfunction
