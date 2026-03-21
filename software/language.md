# Language Design

## Identifiers

 -  :   - Label
 -  $   - Variable
 -  @   - Register

## Instructions

 -  func    (name), (par0), ... {}

 -  def     (name), (type) = (val1);
 -	set     (val0)[i] = (val1)[i];

 -	add     (val1)[i], (val2)[i] = (val0)[i];
 -	sub     (val1)[i], (val2)[i] = (val0)[i];
 -	inc     (val1)[i] = (val0)[i];
 -	dec     (val1)[i] = (val0)[i];
 -	mul     (val1)[i], (val2)[i] = (val0)[i];
 -	div     (val1)[i], (val2)[i] = (val0)[i];
 -	and     (val1)[i], (val2)[i] = (val0)[i];
 -	or      (val1)[i], (val2)[i] = (val0)[i];
