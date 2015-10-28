base = "/home/ubuntu/NXT/brofist/scicos_32/";

ilib_for_link ('bro_comm_blocks', 'bro_comm_blocks.o', [], "c");
ilib_for_link ('bro_sensor_enc', 'bro_sensor_enc.o', [], "c");
ilib_for_link ('bro_motor_enc', 'bro_motor_enc.o', [], "c");

link(base+'libbro_comm_blocks.so',['bro_comm_sens_disp','bro_comm_controller','bro_comm_motor_disp'],'c');
link(base+'libbro_sensor_enc.so',['bro_sensor_enc'],'c');
link(base+'libbro_motor_enc.so',['bro_motor_enc'],'c');


exec bro_sensor_enc.sci;
exec bro_comm_controller.sci;
exec bro_comm_sens_disp.sci;
exec bro_comm_motor_disp.sci;
exec bro_motor_enc.sci;
