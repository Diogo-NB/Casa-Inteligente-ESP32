import { IsBoolean, IsPositive } from 'class-validator';

export class SensorDto {
  @IsBoolean()
  pir: boolean;

  @IsPositive()
  temperature: number;

  @IsPositive()
  humidity: number;
}
