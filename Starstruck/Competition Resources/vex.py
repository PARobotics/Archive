'''
  vex module is used to generate statictics on all teams in an event 
  
  public functions
    event_scout:                generate statistics of team that attends an event, optionaly grouped
    get_team_from_event:        return a list of all teams and information from an event team[iteam][0] is team ID
    get_team_opr:               get opr statistics of a team
    get_team_skills:            get skills statistics of a team
    get_record_of_team_at_event: form records array of all the teams at an event

  class vexEvent:
    global variables:
      url
      xlsxfile
      records
    functions
      get_records
      write_records (ngroup)

  how to use
    from vex import vexEvent                                              # import module/class
    event = vexEvent('http://www.robotevents.com/re-vrc-16-3279.html')    # init event object (from robotevents.com)
    event.get_records()                                                   # form all records 
    event.write_records(0)                                                # write out records to xlsx into single group
    # event.write_records(5)                                                # write out records to xlsx into group of 5 (for worlds)
    
'''
from __future__ import print_function
import csv, xlsxwriter
import glob,os
    
import urllib2
import json
from bs4 import BeautifulSoup
import numpy as np

# __all__ = ["event_scout","get_team_from_event", "get_team_opr", "get_team_skills","get_record_of_team_at_event"]
__all__ = ["vexEvent"]


def get_html_table_row(tr):
  '''
    get_html_table_row parse a row in html <td> </td> pair
    note that 'utf-8' encode things can mess things up 
  '''
  tds = tr.find_all('td')
  # row = [elem.text.encode('utf-8') for elem in tds]
  # row = [elem.text.decode('utf-8') for elem in tds]
  row = [elem.text for elem in tds]
  return row
  
def get_team_from_event(url):
  '''
  vex.get_team_from_event('eventURL') get all the team IDs as a list from eventURL
  e.g. 
    teams = vex.get_team_from_event('http://www.robotevents.com/re-vrc-16-3279.html')
    print(teams)
  '''
  print('Getting team info from event '+url)
  teams = []
  request = urllib2.Request(url)
  response = urllib2.urlopen(request)
  html = response.read()
  # tableId = 3
  tableId = 0
  soup = BeautifulSoup(html)
  table2 = soup.find_all('table')[tableId]
  # print table2
  for tr in table2.find_all('tr'):
    row = get_html_table_row(tr)
    if len(row) != 0: 
      teams.append(row)
  response.close()
  return teams
  
def get_team_opr(team):
  '''
  vex.get_team_opr('teamId') where teamId is a string returns team opr statistics of current season
  as a list 
  e.g. 
    opr = vex.get_team_opr('8900')
    print(opr)
  '''
  url='http://api.vexdb.io/v1/get_rankings?team='+team+'&season=current'
  # print url
  request = urllib2.Request(url)
  response = urllib2.urlopen(request)
  html = response.read()
  data=json.loads(html)
  ngames = data['size']
  opr = []
  for i in xrange(ngames):
    opr.append(float(data['result'][i]['opr']))
  return opr
  # print op

def get_team_skills(team):
  '''
  vex.get_team_skills('teamId') where teamId is a string returns team's max skills scores
  (an array of 2 with 1st being robot skills and second being programming skills)
  e.g. 
    skills = vex.get_team_skills('8900')
    print(skills)
  note
    type=0: driving skills
    type=1: programming skills
    type=2: robot skills
  '''
  url='http://api.vexdb.io/v1/get_skills?team='+team+'&season=current'
  # print url
  request = urllib2.Request(url)
  response = urllib2.urlopen(request)
  html = response.read()
  data=json.loads(html)
  ngames = data['size']
  skills=[0,0,0]
  for i in xrange(ngames):
    id = int(data['result'][i]['type'])
    val = float(data['result'][i]['score'])
    if (skills[id]<val): skills[id]=val
  return skills
  # print op    
    

def get_record_of_team_at_event(url):
  '''
  vex.get_record_of_team_at_event('eventURL') gets records of all the teams at an event
  e.g. 
    rec = vex.get_record_of_team_at_event('http://www.robotevents.com/re-vrc-16-3279.html')
    print(rec)
  '''
  records = [['Team','Name','Org','Location','Drv_skills','Prg_skills','Robot_skills','Opr_max','Opr_last','Opr-1','Opr-2','Opr-3','Opr-4','Opr-5']]
  print (url)
  teams = get_team_from_event(url)
  nTeams = len(teams)
  print ('Compiling team records ...')
  # for iteam in range(10,20):
  for iteam in range(nTeams):
    team   = teams[iteam][0]
    print ('  %3d/%3d %s' % (iteam,nTeams,team))
    skills = get_team_skills(team)
    # print('skills=',skills)
    opr    = get_team_opr(team)
    # print('opr=',opr)
    row    = teams[iteam]
    row.extend(skills)
    if (len(opr)>0):
      row.append(np.max(opr))
      row.extend(opr)
    # print(row)
    records.append(row)
  return records
  
def write_records_to_xlsx(xlsxfile,records,ngroup):
  wb = xlsxwriter.Workbook(xlsxfile)
  bold = wb.add_format({'bold': True})
  print('Generating ' + xlsxfile + ' ...')
  for igrp in xrange(ngroup+1):
    if (igrp==0):
      sheetname="ALL"
      crecords = records
    else:
      if (ngroup==1): break
      sheetname = "Grp_"+str(igrp)
      crecords = []
      crecords.append(records[0])
      crecords.extend(records[igrp::ngroup])
    print('  sheet: ' + sheetname)
    ws = wb.add_worksheet(sheetname)
    
    for r,row in enumerate(crecords) :
      for c, col in enumerate(row):
        if (r==0 or c==0):
          ws.write(r, c, col, bold)
        else:
          ws.write(r, c, col)
  wb.close()
    
  
  
class vexEvent:
  '''
  how to use
    from vex import vexEvent                                              # import module/class
    event = vexEvent('http://www.robotevents.com/re-vrc-16-3279.html')    # init event object
    event.get_records()                                                   # form all records 
    event.write_records(5)                                                # write out records to xlsx into group of 5
  '''
  def __init__(self, url):
    self.url = url
    self.xlsxfile = url.split('/')[-1].split('.')[0]+'.xlsx'
    self.records = []
  def get_records(self):
    self.records = get_record_of_team_at_event(self.url)
  def write_records(self,ngroup=1):
    write_records_to_xlsx(self.xlsxfile,self.records,ngroup)
  

 