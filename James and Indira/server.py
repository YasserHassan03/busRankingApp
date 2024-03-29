import socket
import csv
#import csv
import threading 
import decimal
import numpy as np
import shutil
from statistics import mean

def smoothness_score(x_vals, y_vals, z_vals, time_interval):
    x_jerk_list = [1,1,1]
    y_jerk_list = [1,1,1]
    z_jerk_list = [1,1,1]
    x_jerk_magnitudes = [1,1,1]
    y_jerk_magnitudes = [1,1,1]
    z_jerk_magnitudes = [1,1,1]
    x_smoothness_scores = []
    y_smoothness_scores = []
    z_smoothness_scores = []
    
    for i in range(1, len(x_vals)) and range(1, len(y_vals)) and range(1, len(z_vals)):
        if x_vals[i] and y_vals[i] and z_vals[i] != []:
            
            
            x_jerk = decimal.Decimal((x_vals[i] - x_vals[i-1]) / time_interval)
            y_jerk = decimal.Decimal((y_vals[i] - y_vals[i-1]) / time_interval)
            z_jerk = decimal.Decimal((z_vals[i] - z_vals[i-1]) / time_interval)

            x_jerk_list.append(x_jerk)
            y_jerk_list.append(y_jerk)
            z_jerk_list.append(z_jerk)

            x_jerk_magnitude = abs(decimal.Decimal(x_jerk))
            y_jerk_magnitude = abs(decimal.Decimal(y_jerk))
            z_jerk_magnitude = abs(decimal.Decimal(z_jerk))

            x_jerk_magnitudes.append(x_jerk_magnitude)
            y_jerk_magnitudes.append(y_jerk_magnitude)
            z_jerk_magnitudes.append(z_jerk_magnitude)

            window_size = 5

            for i in range(len(x_jerk_magnitudes)):
                start = max(0, i-window_size)
                end = min(len(x_jerk_magnitudes), i+window_size)
                x_jerk_window = x_jerk_magnitudes[start:end]
                y_jerk_window = y_jerk_magnitudes[start:end]
                z_jerk_window = z_jerk_magnitudes[start:end]

                try:
                    x_smoothness_score = 1 / decimal.Decimal(np.mean(x_jerk_window))
                    y_smoothness_score = 1 / decimal.Decimal(np.mean(y_jerk_window))
                    z_smoothness_score = 1 / decimal.Decimal(np.mean(z_jerk_window))
                except:
                    x_smoothness_score = 0
                    y_smoothness_score = 0
                    z_smoothness_score = 0

                x_smoothness_scores.append(x_smoothness_score)
                y_smoothness_scores.append(y_smoothness_score)
                z_smoothness_scores.append(z_smoothness_score)

        else:
            pass
    average_x_smoothness = decimal.Decimal(np.mean(x_smoothness_scores))
    average_y_smoothness = decimal.Decimal(np.mean(y_smoothness_scores))
    average_z_smoothness = decimal.Decimal(np.mean(z_smoothness_scores))
    
    x_smoothness_score = 1 / average_x_smoothness
    y_smoothness_score = 1 / average_y_smoothness
    z_smoothness_score = 1 / average_z_smoothness
    
    max_smoothness_score = 1 / decimal.Decimal(0.1)  # The maximum possible jerk magnitude is 0.1 m/s^2, assuming a perfectly smooth ride
    avrg_smoothness = (x_smoothness_score + y_smoothness_score + z_smoothness_score) / 3
    normalised_smoothness_score = avrg_smoothness / max_smoothness_score
    
    return decimal.Decimal(normalised_smoothness_score)

def process_file(filename):
    with open(filename, "r", encoding="utf-8") as file:
        contents = file.readlines()[1:]  # skip the first line (assuming it's a header)
        x_vals = [1]
        y_vals = [1]
        z_vals = [1]
        for line in contents:
            values = line.strip().split(",")
            if len(values) != 3:  # skip lines that don't contain exactly 3 values
                continue
            try:
                x_vals.append(int(values[0]))
                y_vals.append(int(values[1]))
                z_vals.append(int(values[2]))
            except ValueError:  # skip lines that contain non-numeric data
                continue
    file.close()
    return x_vals, y_vals, z_vals


    #file.close()



print("We're in server now..")
with open ('data.txt','w+',newline='\r') as file:
    file.close()
with open ('journey.txt','w+',newline='\r') as file:
    file.close()
with open ('latest.txt','w+',newline='\r') as file:
    file.close()
#deadfile= open('data.txt','w')
#deadfile.close()
#clear file for new data
#select port for server
resultscale = '000000' #test score val initial
server_port=12005
#create welcoming socket
welcome_socket=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
#bind server to local host
welcome_socket.bind(('0.0.0.0',server_port))
welcome_socket.listen(1)
#ready message
print("server running on port: ",server_port)
cmsg=''
#now server side loop

#print(score())

 #send score to client 
while True:   
    connection_socket,caddr=welcome_socket.accept()
    x_vals, y_vals, z_vals = process_file("latest.txt")
    #print(len(x_vals),len(y_vals),len(z_vals))
    if len(x_vals) + len(y_vals) + len(z_vals) > 3:
        resultround = decimal.Decimal((smoothness_score(x_vals, y_vals, z_vals, 1.0)))
        score=[]
        resultround = round(resultround, 6)
        try:
            score.append(resultround)
            resultscale=round(mean(score)*100000)
            resultscale=str(resultscale)
            if len(resultscale)==5:
                resultscale='0'+resultscale
            elif len(resultscale)==4:
                resultscale='00'+resultscale
            print(resultscale)
        except ValueError:
            continue
    connection_socket.send((str(resultscale)).encode())
    cmsg=connection_socket.recv(1024)     
    cmsg=cmsg.decode()
    if (len(cmsg)==0):
        print("finished j")
        shutil.copyfile('data.txt','journey.txt')
        print("written")
        with open('data.txt','w',newline='\r') as file:
            file.close()    
        resultscale = '000000'    
    print(cmsg.split('\r'))
    with open ('data.txt','a',newline='\r') as file:
        file.write(cmsg)
        file.close()
    with open ('latest.txt', 'w+', newline='\r') as file:
        file.write(cmsg)
        file.close()
    

    
        
