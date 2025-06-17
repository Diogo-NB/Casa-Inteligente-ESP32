import { IsNumber } from 'class-validator';

export class SensorDto {
  @IsNumber()
  distance: number;

  @IsNumber()
  temperature: number;

  @IsNumber()
  humidity: number;
}
