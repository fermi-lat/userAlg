# -*- python -*-
# $Header: /nfs/slac/g/glast/ground/cvs/userAlg/SConscript,v 1.13 2012/01/20 22:51:39 jrb Exp $ 
# Authors: T.Burnett <tburnett@u.washington.edu>
# Version: userAlg-06-07-01
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

progEnv.Tool('ntupleWriterSvcLib')

userApp = progEnv.GaudiProgram('userApp', 
			       listFiles(['src/*.cxx']), test=1,
			       package='userAlg')
test_userAlg = progEnv.GaudiProgram('test_userAlg', 
				    ['src/UserAlg.cxx', 
				     'src/RegisterSource.cxx'], 
				    test = 1, package='userAlg')

progEnv.Tool('registerTargets', package = 'userAlg',
	     testAppCxts = [[test_userAlg, progEnv], 
			    [userApp,progEnv]],
	     jo = listFiles(['src/*.txt', 
			     'src/test/jobOptions.txt']),
         xml = listFiles(['xml/*.xml']))




