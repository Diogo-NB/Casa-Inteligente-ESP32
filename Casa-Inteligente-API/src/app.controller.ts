import { Body, Controller, Post } from '@nestjs/common';
import { SensorDto } from './dto/sensor.dto';
import { ActuatorsStateDto } from './dto/actuators-state.dto';

@Controller()
export class AppController {
  constructor() {}

  static count = 0; // TODO - REMOVER
  static lastSensorData: SensorDto | null = null; // TODO - ARMAZENAR NO BANCO E NÃO EM MEMÓRIA

  @Post('sensors')
  postSensors(@Body() dto: SensorDto): void {
    console.log('Got sensor data:', dto);
    AppController.lastSensorData = dto; // TODO - INSERT NO BANCO
  }

  @Post('actuators/state')
  getActuatorsState(
    @Body() currentState: ActuatorsStateDto,
  ): Partial<ActuatorsStateDto> {
    console.log('Got current state:', AppController.count++, currentState);
    if (!AppController.lastSensorData) {
      return {
        lamp1: false,
        lamp2: false,
        lamp3: false,
        lamp4: false,
        fan1: false,
        fan2: false,
        general1: false,
        general2: false,
      };
    }

    const { distance, temperature, humidity } = AppController.lastSensorData;

    return {
      lamp1: !currentState.lamp1,
    }; // TODO - LÓGICA DE ATUALIZAR ESP
  }
}
