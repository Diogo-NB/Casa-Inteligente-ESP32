import { Body, Controller, Get, Post } from '@nestjs/common';
import { SensorDto } from './dto/sensor.dto';
import { ActuatorsStateDto } from './dto/actuators-state.dto';

const HYSTERESIS = 1.0;

@Controller()
export class AppController {
  constructor() {}

  // TODO - ARMAZENAR NO BANCO E NÃO EM MEMÓRIA
  static lastSensorData: SensorDto | null = null;
  static setPoint: Partial<SensorDto> = {
    distance: 10,
  };
  static lamp1State: boolean = false;

  @Post('set-point/temperature')
  postSetPointTemperature(@Body('temperature') temperature: number): void {
    AppController.setPoint.temperature = temperature;
  }

  @Post('lamp1')
  lamp1SwitchOn(@Body('state') state: boolean): void {
    AppController.lamp1State = state;
  }

  @Get('temperature')
  getTemperature(): number {
    return AppController.lastSensorData?.temperature ?? 0;
  }

  @Post('sensors')
  postSensors(@Body() dto: SensorDto): void {
    AppController.lastSensorData = dto; // TODO - INSERT NO BANCO
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
        fan1: false,
        fan2: false,
        general1: false,
        general2: false,
      };
    }

    const { distance, temperature, humidity } = AppController.lastSensorData;
    const setPoint = AppController.setPoint;

    let fan1: boolean | null = null;
    let lamp2: boolean | null = null;

    if (setPoint.temperature) {
      if (temperature >= setPoint.temperature + HYSTERESIS) {
        fan1 = true;
      }

      if (temperature <= setPoint.temperature - HYSTERESIS) {
        fan1 = false;
      }
    }

    if (setPoint.distance) {
      lamp2 = distance <= setPoint.distance;
    }

    return {
      lamp1: AppController.lamp1State,
      lamp2: lamp2 ?? currentState.lamp2,
      fan1: fan1 ?? currentState.fan1,
    }; // TODO - LÓGICA DE ATUALIZAR ESP
  }
}
