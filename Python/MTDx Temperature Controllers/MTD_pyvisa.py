#Tested with MTD415T, MTDEVAL1,TECF2S,TH10K
import pyvisa
import time
def main():
    rm = pyvisa.ResourceManager()


    resources=rm.list_resources()
    print('Resources: ',resources)
    if len(resources)==0:
           sys.exit("No device found")
    else:
        #device address is currently ASRL12 
        MTD = rm.open_resource('ASRL12',baud_rate=115200,data_bits=8)

        MTD.timeout = 2000

        MTD.read_termination='\n'
        MTD.write_termination='\n'


        print(MTD.query('m?'))#Reads the version of hardware and firmware
        MTD.write('L1500')#Sets the TEC current limit t0 1500 mA
        print('TEC current limit: ',MTD.query('L?'),'mA')#Reads the TEC current limit

        MTD.write('T25000')
        print('Set temperature:',float(MTD.query('T?'))/1000,'°C')

        MTD.write('P1000')#set P share [mA/K]
        print('P share: ',MTD.query('P?'),' mA/K')
        MTD.write('I0')#set I share [mA/K s]
        print('I share: ',MTD.query('I?'),'mA/K s')
        MTD.write('D0')#set D share [mA s/K]
        print('D share: ',MTD.query('D?'),'mA s/K')       
        MTD.write('C50')#set cycle time [ms]
        print('Cycle time: ',MTD.query('C?'),'ms\n')

        for i in range(10):
            print('Current temperature: ',float(MTD.query('Te?'))/1000,'°C')#print current temperature
            time.sleep(1)
        
        

if __name__ == "__main__":
    main()
