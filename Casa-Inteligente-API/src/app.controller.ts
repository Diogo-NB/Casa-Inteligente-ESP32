import { Body, Controller, Post } from '@nestjs/common';
import { SensorDto } from './dto/sensor.dto';
import { ActuatorsStateDto } from './dto/actuators-state.dto';

@Controller()
export class AppController {
  constructor() {}

  static lastSensorData: SensorDto | null = null; // TODO - ARMAZENAR NO BANCO E NÃO EM MEMÓRIA

  @Post('sensors')
  postSensors(@Body() body: SensorDto): void {
    AppController.lastSensorData = body; // TODO - INSERT NO BANCO
  }

  @Post('actuators/state')
  getActuatorsState(
    @Body() currentState: ActuatorsStateDto,
  ): Partial<ActuatorsStateDto> {
    if (!AppController.lastSensorData) {
      return {
        lamp1: false,
        lamp2: false,
        lamp3: false,
        lamp4: false,
        fan1: true,
        fan2: false,
        general1: false,
        general2: false,
      };
    }

    const { pir, temperature, humidity } = AppController.lastSensorData;

    return {
      lamp1: !currentState.lamp1,
      lamp3: !currentState.lamp3,
      fan1: pir ? currentState.fan2 : false,
      fan2: pir ? currentState.fan1 : false,
      general1: pir,
      general2: pir,
    }; // TODO - LÓGICA DE ATUALIZAR ESP
  }
}
