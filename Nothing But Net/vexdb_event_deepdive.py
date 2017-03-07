from __future__ import print_function
import csv
import urllib2
from bs4 import BeautifulSoup
import argparse

# to use the script, modify the event url link and run to generate 
# a csv file (with event id as file name) with
# a team list and breakdown of team statistics
    
def get_row(tr):
  tds = tr.find_all('td')
  row = [elem.text.encode('utf-8') for elem in tds]
  return row
  
def parse_event_team_table(html):
  # table id in the main event table is 3
  tableId = 3
  soup = BeautifulSoup(html)
  table2 = soup.find_all('table')[tableId]
  # for tr in table2.find_all('tr')[2:]:
  for tr in table2.find_all('tr'):
    row = get_row(tr)
    if len(row) != 0: 
      records.append(row)
      Teams.append(row[1])
      return Teams

def parse_event(url):
  # try upto 5 subpages till it finds an empty table with "No data found."
  noMoreTxt = 'No data found.'
  try:
    for subpage in [1,2,3,4,5]:
      urllink=url+'?p='+str(subpage)
      request = urllib2.Request(urllink)
      response = urllib2.urlopen(request)
      html = response.read()
      tableId = 3
      soup = BeautifulSoup(html)
      table2 = soup.find_all('table')[tableId]
      for tr in table2.find_all('tr'):
        row = get_row(tr)
        if len(row) != 0: 
          if noMoreTxt in row: 
            raise StopIteration
          records.append(row)
          Teams.append(row[1])
      response.close()
  except StopIteration: 
    pass
  return Teams

def parse_team(teamNo, query):
  # query = rankings/robot/programming/awards etc
  # the max scores (OP, robot, programming) are bing tracked
  maxv = 0
  url='http://vexdb.io/teams/view/'+teamNo+'?t='+query
  tableID = 3
  request = urllib2.Request(url)
  response = urllib2.urlopen(request)
  html = response.read()
  tableId = 3
  soup = BeautifulSoup(html)
  table2 = soup.find_all('table')[tableId]
  for tr in table2.find_all('tr'):
    row = get_row(tr)
    if len(row) != 0: 
      row.insert(0,' ')
      row.insert(0,' ')
      records.append(row)
      # find max OP
      if query.lower()=="rankings": 
        if len(row)>=9 and float(row[9]) > maxv: maxv = float(row[9])
      if query.lower()=="robot": 
        if len(row)>=5 and float(row[5]) > maxv: maxv = float(row[5])
      if query.lower()=="programming": 
        if len(row)>=5 and float(row[5]) > maxv: maxv = float(row[5])
  response.close()
  return maxv
  
  
############################################################
# records and Teams are global variables
# url is also a global variable which is the web address of the event  
records = []
Teams = []
 
# specify the event URL and query that will list all the teams in the table
# url = 'http://vexdb.io/events/view/RE-VRC-16-4016'       # california states
url = 'http://vexdb.io/events/view/RE-VRC-16-4227'       # S. New England
parse_event(url)
print ('Teams = '+' '.join(Teams))


# Now loop through all the teams and query ranking, robot, programming skills 
for iteam in range(len(Teams)):
# for iteam in range(2):
  teamNo = Teams[iteam]
  print ('processing Teams '+teamNo)
  records.append([teamNo]) 
  
  records.append([' ','rankings'])
  maxv = parse_team(teamNo,'rankings')
  records[iteam].append(maxv)         # add max OP to team records
  
  records.append([' ','robot skills'])
  maxv = parse_team(teamNo,'robot')
  records[iteam].append(maxv)         # add max robot to team records
  
  records.append([' ','programming skills']) 
  maxv = parse_team(teamNo,'programming')
  records[iteam].append(maxv)         # add max programming to team records
  
# Now write to a csv file that can be used to edit as xlsx file later
with open(url.split('/')[-1]+'.csv', 'wb') as f:
    writer = csv.writer(f)
    writer.writerows(records)
    
    
    