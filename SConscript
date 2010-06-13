# -*- python -*-
# $Header: /nfs/slac/g/glast/ground/cvs/userAlg/SConscript,v 1.6 2010/06/11 00:48:53 jrb Exp $ 
# Authors: T.Burnett <tburnett@u.washington.edu>
# Version: userAlg-06-06-05
import os
Import('baseEnv')
Import('listFiles')
Import('packages')
progEnv = baseEnv.Clone()
##libEnv = baseEnv.Clone()

progEnv.Tool('FluxSvcLib')
progEnv.Tool('identsLib')
progEnv.Tool('EventLib')
progEnv.Tool('facilitiesLib')
progEnv.Tool('GuiSvcLib')
progEnv.Tool('guiLib')
progEnv.Tool('astroLib')
#progEnv.Tool('GleamLib')
progEnv.Tool('ntupleWriterSvcLib')

if baseEnv['PLATFORM'] != 'win32':
        progEnv.AppendUnique(LINKFLAGS = ['-u GuiSvc_loadRef'])

if baseEnv['PLATFORM'] == 'win32':
        progEnv.AppendUnique(LINKFLAGS =['/include:_GuiSvc_loadRef'])
        progEnv.AppendUnique(LINKFLAGS=['/subsystem:windows'])  #from macro guiapp_linkopts in ../gui/cmt/requirements


userApp = progEnv.GaudiProgram('userApp', listFiles(['src/*.cxx']), test=1,
			       package='userAlg')
test_userAlg = progEnv.GaudiProgram('test_userAlg', ['src/UserAlg.cxx', 'src/RegisterSource.cxx'], test = 1, package='userAlg')

progEnv.Tool('registerTargets', package = 'userAlg',
	     testAppCxts = [[test_userAlg, progEnv], [userApp,progEnv]],
	     jo = listFiles(['src/*.txt', 'src/test/jobOptions.txt']))




