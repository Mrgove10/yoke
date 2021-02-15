export class simData {
  public lat: number = 0;
  public long: number = 0;
  public speed: number = 0;
  public alt: number = 0;
  public bank: number = 0;
  public pitch: number = 0;

  public simData() { }

  public getArduinoArray = (): Array<any> => {
    var strData = [
      this.lat,
      this.long,
      this.long,
      this.alt,
      this.bank,
      this.pitch,
    ]
    return strData;
  }
}
