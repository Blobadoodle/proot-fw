// This is a Flipper Zero script to allow you to control ProotFW over serial!

let serial = require('serial');
let submenu = require('submenu');
let notify = require('notification');

serial.setup('usart', 115200);

let commands = [];
commands[0] = '+';
commands[1] = '>';
commands[2] = 'b';
commands[3] = 'g';

while(true) {
	submenu.addItem('Cycle brightness', 0);
	submenu.addItem('Cycle expression', 1);
	submenu.addItem('Blink', 2);
	submenu.addItem('Glitch', 3);
	submenu.addItem('Exit', 4);

	submenu.setHeader('ProotFW Controller');

	let result = submenu.show();
	if(result === undefined || result === 4) {
		print('ProotFW by @blobadoodle');
		break;
	} else {
		serial.write(commands[result]);
		notify.blink('blue', 'short');
	}
}