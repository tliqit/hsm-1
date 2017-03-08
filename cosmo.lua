cosmo=require'cosmo'
v={rank='ace',suit='spades'}
tpl='$rank of $suit'
s=cosmo.fill(tpl,v);
s1=cosmo.f('$rank of $suit'){rank='ace',suit='spades'}
s2=cosmo.f('$cards|1|rank of $cards|1|suit'){cards={{rank='ace',suit='spades'}}}
s3=cosmo.f("$m{rank='ace',suit='spades'}"){m=function(arg)
  return arg.rank..' of '..arg.suit
end}
print(s,s1,s2,s3)

s4=cosmo.f("$docards[[$rank of $suit, ]]"){docards={{rank="ace",suit="spades"},{rank="Queen", suit="Diamonds"}, {rank="10", suit="Hearts"}}}
print(s4)

mycards={{rank="ace",suit="spades"},{rank="Queen", suit="Diamonds"}, {rank="10", suit="Hearts"}}
s5=cosmo.f("$docards[[$rank of $suit, ]]"){docards=function()
  for k,v in ipairs(mycards) do
    cosmo.yield{rank=v.rank,suit=v.suit}
  end
end}
print(s5)

s6=cosmo.f("$docards[[$rank of $suit]],[[, $rank of $suit]],[[, and $rank of $suit]]"){docards=function()
  for k,v in ipairs(mycards) do
    local t
    if k==3 then
      t=3
    elseif k~=1 then
      t=2
    end
    cosmo.yield{rank=v.rank,suit=v.suit,_template=t}
  end
end}
print(s6)

players={'ken','rego'}
cards={}
cards['ken']=mycards
cards['rego']={{rank='ring',suit='diamonds'}}
s7=cosmo.f("$doplayers[=[$player has $docards[[$rank of $suit]],[[, $rank of $suit]],[[, and $rank of $suit]]\n]=]"){doplayers=function()
  for k,p in ipairs(players) do
    cosmo.yield{
      player=p,
      docards=function()
        local t
        for k,v in ipairs(cards[p]) do
          if k==3 then
            t=3
          elseif k~=1 then
            t=2
          end
          cosmo.yield{rank=v.rank,suit=v.suit,_template=t}
        end
      end
    }
  end
end}
print(s7)

s8=cosmo.f("$if{math.fmod(x,4)==0,target='world'}[[hello $target]],[[hi $target]]"){
  math=math,
  x=4,
  ['if']=cosmo.cif
}
print(s8)

s9=cosmo.f("$doplayers[=[$player: $n cards $if{$needmore,_n=3-$n}[[(need $_n more)]],[[(no need more)]]\n]=]"){
  doplayers=function()
    for k,p in ipairs(players) do
      cosmo.yield{
        player=p,
        n=#cards[p],
        needmore=#cards[p]<3,
        ['if']=cosmo.cif
      }
    end
  end
}
print(s9)





