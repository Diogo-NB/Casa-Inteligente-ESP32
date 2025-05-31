import { IsBoolean } from 'class-validator';

export class ActuatorsStateDto {
  @IsBoolean()
  lamp1: boolean;

  @IsBoolean()
  lamp2: boolean;

  @IsBoolean()
  lamp3: boolean;

  @IsBoolean()
  lamp4: boolean;

  @IsBoolean()
  fan1: boolean;

  @IsBoolean()
  fan2: boolean;

  @IsBoolean()
  general1: boolean;

  @IsBoolean()
  general2: boolean;
}
