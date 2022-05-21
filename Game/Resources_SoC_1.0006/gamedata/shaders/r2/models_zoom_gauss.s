function normal   (shader, t_base, t_second, t_detail)
	shader:begin	("model_def_lplanes", "models_zoom_gauss")
      : fog    		(false)
      : zb     		(true,false)
      : blend   	(true,blend.srcalpha,blend.one)
      : aref    	(true,0)
      : sorting		(2, true)
	shader:sampler  ("s_base")		:texture	(t_base)
	shader:sampler  ("s_vp2")		:texture	("$user$viewport2")
end
